// Reaktoro is a unified framework for modeling chemically reactive systems.
//
// Copyright (C) 2014-2015 Allan Leal
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

#pragma once

// C++ includes
#include <memory>

// Reaktoro includes
#include <Reaktoro/Math/Matrix.hpp>

namespace Reaktoro {

// Forward declarations
class ChemicalSystem;
class Partition;
class EquilibriumReactions;

/// A class that defines the mass balance conditions for equilibrium calculations.
class EquilibriumBalance
{
public:
    /// Construct an EquilibriumBalance instance with given chemical system.
    EquilibriumBalance(const ChemicalSystem& system);

    /// Construct an EquilibriumBalance instance with given chemical system and partition.
    EquilibriumBalance(const ChemicalSystem& system, const Partition& partition);

    /// Construct an EquilibriumBalance instance with customized equilibrium reactions.
    EquilibriumBalance(const EquilibriumReactions& reactions);

    /// Construct a copy of an EquilibriumBalance instance
    EquilibriumBalance(const EquilibriumBalance& other);

    /// Destroy this EquilibriumBalance instance
    virtual ~EquilibriumBalance();

    /// Assign other EquilibriumBalance instance into this.
    auto operator=(EquilibriumBalance other) -> EquilibriumBalance&;

    /// Return the regularized mass balance matrix.
    /// The regularized mass balance matrix is an alternative
    /// to the elemental balance matrix that reduces round-off error
    /// issues when some species have very low amounts.
    auto regularizedMatrix() const -> const Matrix&;

    /// Return the regularized mass balance vector.
    /// The regularized mass balance matrix is an alternative
    /// to the elemental balance matrix that reduces round-off error
    /// issues when some species have very low amounts.
    /// @param b The vector of molar amounts of the elements.
    auto regularizedVector(Vector b) const -> Vector;

private:
    struct Impl;

    std::unique_ptr<Impl> pimpl;
};


} // namespace Reaktoro
