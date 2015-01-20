// Reaktor is a C++ library for computational reaction modelling.
//
// Copyright (C) 2014 Allan Leal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "OptimumSolverIpnewton.hpp"

// Reaktor includes
#include <Reaktor/Common/Exception.hpp>
#include <Reaktor/Common/Outputter.hpp>
#include <Reaktor/Common/TimeUtils.hpp>
#include <Reaktor/Math/MathUtils.hpp>
#include <Reaktor/Optimization/KktSolver.hpp>
#include <Reaktor/Optimization/OptimumProblem.hpp>
#include <Reaktor/Optimization/OptimumOptions.hpp>
#include <Reaktor/Optimization/OptimumResult.hpp>
#include <Reaktor/Optimization/OptimumState.hpp>
#include <Reaktor/Optimization/Utils.hpp>

namespace Reaktor {

struct OptimumSolverIpnewton::Impl
{
    Vector dx, dy, dz;

    Vector a, b;
    KktSolver kkt;

    Outputter outputter;

    auto solve(const OptimumProblem& problem, OptimumState& state, const OptimumOptions& options) -> OptimumResult;
};

auto OptimumSolverIpnewton::Impl::solve(const OptimumProblem& problem, OptimumState& state, const OptimumOptions& options) -> OptimumResult
{
    // Start timing the calculation
    Time begin = time();

    // The result of the calculation
    OptimumResult result;

    // Define some auxiliary references to variables
    auto& x = state.x;
    auto& y = state.y;
    auto& z = state.z;
    auto& f = state.f;
    auto& g = state.g;
    auto& H = state.H;
    auto& h = state.h;
    auto& A = state.A;

    // Define some auxiliary references to parameters
    const auto& n         = problem.numVariables();
    const auto& m         = problem.numConstraints();
    const auto& tolerance = options.tolerance;
    const auto& mu        = options.ipnewton.mu;
    const auto& mux       = options.ipnewton.mux;
    const auto& tau       = options.ipnewton.tau;

    // Ensure the initial guesses for `x` and `y` have adequate dimensions
    if(x.size() != n) x = zeros(n);
    if(y.size() != m) y = zeros(m);
    if(z.size() != n) z = zeros(n);

    // Ensure the initial guess for `x` is inside the feasible domain
    x = max(x, mux*mu*ones(n));

    // Ensure the initial guess for `z` is inside the feasible domain
    z = (z.array() > 0).select(z, mu/x);

    // The transpose representation of matrix `A`
    const auto At = A.transpose();

    // The alpha step sizes used to restric the steps inside the feasible domain
    double alphax, alphaz, alpha;

    // The optimality, feasibility, centrality and total error variables
    double errorf, errorh, errorc, error;

    // The function that outputs the header and initial state of the solution
    auto output_header = [&]()
    {
        if(not options.output.active) return;

        outputter.setOptions(options.output);

        outputter.addEntry("iter");
        outputter.addEntries("x", n);
        outputter.addEntries("y", m);
        outputter.addEntries("z", n);
        outputter.addEntry("f(x)");
        outputter.addEntry("h(x)");
        outputter.addEntry("errorf");
        outputter.addEntry("errorh");
        outputter.addEntry("errorc");
        outputter.addEntry("error");
        outputter.addEntry("alpha");
        outputter.addEntry("alphax");
        outputter.addEntry("alphaz");

        outputter.outputHeader();
        outputter.addValue(result.iterations);
        outputter.addValues(x);
        outputter.addValues(y);
        outputter.addValues(z);
        outputter.addValue(f);
        outputter.addValue(norminf(h));
        outputter.addValue("---");
        outputter.addValue("---");
        outputter.addValue("---");
        outputter.addValue("---");
        outputter.addValue("---");
        outputter.addValue("---");
        outputter.addValue("---");
        outputter.outputState();
    };

    // The function that outputs the current state of the solution
    auto output_state = [&]()
    {
        if(not options.output.active) return;

        outputter.addValue(result.iterations);
        outputter.addValues(x);
        outputter.addValues(y);
        outputter.addValues(z);
        outputter.addValue(f);
        outputter.addValue(norminf(h));
        outputter.addValue(errorf);
        outputter.addValue(errorh);
        outputter.addValue(errorc);
        outputter.addValue(error);
        outputter.addValue(alpha);
        outputter.addValue(alphax);
        outputter.addValue(alphaz);
        outputter.outputState();
    };

    // The function that updates the objective and constraint state
    auto update_state = [&]()
    {
        f = problem.objective(x);
        g = problem.objectiveGrad(x);
        h = problem.constraint(x);
        A = problem.constraintGrad(x);
    };

    // The function that computes the Newton step
    auto compute_newton_step = [&]()
    {
        // Pre-decompose the KKT equation based on the Hessian scheme
        H = problem.objectiveHessian(x, g);
        kkt.decompose(state);

        // Compute the right-hand side vectors of the KKT equation
        a.noalias() = -(g - At*y - mu/x);
        b.noalias() = -h;

        // Compute `dx` and `dy` by solving the KKT equation
        kkt.solve(a, b, dx, dy);

        // Compute `dz` with the already computed `dx`
        dz = (mu - z % dx)/x - z;

        // Update the time spent in linear systems
        result.time_linear_systems += kkt.info().time_solve;
        result.time_linear_systems += kkt.info().time_decompose;
    };

    // The function that performs an update in the iterates
    auto update_iterates = [&]()
    {
        alphax = fractionToTheBoundary(x, dx, tau);
        alphaz = fractionToTheBoundary(z, dz, tau);
        alpha  = std::min(alphax, alphaz);

        if(options.ipnewton.uniform_newton_step)
        {
            x += alpha * dx;
            y += alpha * dy;
            z += alpha * dz;
        }
        else
        {
            x += alpha * dx;
            y += dy;
            z += alphaz * dz;
        }
    };

    // The function that computes the current error norms
    auto update_errors = [&]()
    {
        // Calculate the optimality, feasibility and centrality errors
        errorf = norminf(g - At*y - z);
        errorh = norminf(h);
        errorc = norminf(x%z - mu);

        // Calculate the maximum error
        error = std::max({errorf, errorh, errorc});
        result.error = error;
    };

    update_state();
    output_header();

    do
    {
        ++result.iterations;
        compute_newton_step();
        update_iterates();
        update_state();
        update_errors();
        output_state();
    } while(error > tolerance and result.iterations < options.max_iterations);

    outputter.outputHeader();

    if(result.iterations < options.max_iterations)
        result.succeeded = true;

    // Finish timing the calculation
    result.time = elapsed(begin);

    return result;
}

OptimumSolverIpnewton::OptimumSolverIpnewton()
: pimpl(new Impl())
{}

OptimumSolverIpnewton::OptimumSolverIpnewton(const OptimumSolverIpnewton& other)
: pimpl(new Impl(*other.pimpl))
{}

OptimumSolverIpnewton::~OptimumSolverIpnewton()
{}

auto OptimumSolverIpnewton::operator=(OptimumSolverIpnewton other) -> OptimumSolverIpnewton&
{
    pimpl = std::move(other.pimpl);
    return *this;
}

auto OptimumSolverIpnewton::solve(const OptimumProblem& problem, OptimumState& state) -> OptimumResult
{
    return pimpl->solve(problem, state, {});
}

auto OptimumSolverIpnewton::solve(const OptimumProblem& problem, OptimumState& state, const OptimumOptions& options) -> OptimumResult
{
    return pimpl->solve(problem, state, options);
}

} // namespace Reaktor