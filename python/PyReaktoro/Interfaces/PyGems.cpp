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
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "PyGems.hpp"

// Boost includes
#include <boost/python.hpp>
namespace py = boost::python;

// Reaktoro includes
#include <Reaktoro/Interfaces/Gems.hpp>

namespace Reaktoro {

auto export_Gems() -> void
{
    py::class_<GemsOptions>("GemsOptions")
        .def_readwrite("warmstart", &GemsOptions::warmstart)
        ;

    py::class_<Gems, py::bases<Interface>>("Gems")
        .def(py::init<>())
        .def(py::init<std::string>())
        .def("setOptions", &Gems::setOptions)
        .def("equilibrate", &Gems::equilibrate)
        .def("converged", &Gems::converged)
        .def("numIterations", &Gems::numIterations)
        .def("elapsedTime", &Gems::elapsedTime)
        ;
}

} // namespace Reaktoro
