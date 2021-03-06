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
#include <map>
#include <memory>
#include <string>
#include <vector>

// Reaktoro includes
#include <Reaktoro/Math/Matrix.hpp>
#include <Reaktoro/Common/ThermoScalar.hpp>

namespace Reaktoro {

// Forward declarations
class Element;

/// A type used to describe a species and its attributes.
/// The Species class is used to represent a species. It is an important
/// class in the library, since it defines fundamental attributes of a general
/// species such as its elemental formula, electrical charge and molar mass.
/// @see Phase
/// @ingroup Core
class Species
{
public:
    /// Construct a default Species instance.
    Species();

    /// Set the name of the species.
    auto setName(std::string name) -> void;

    /// Set the formula of the species.
    auto setFormula(std::string formula) -> void;

    /// Set the elements of the species.
    auto setElements(const std::map<Element, double>& elements) -> void;

    /// Return the number of elements of the species.
    auto numElements() const -> unsigned;

    /// Return the name of the species.
    auto name() const -> std::string;

    /// Return the formula of the species.
    auto formula() const -> std::string;

    /// Return the elements that compose the species and their coefficients.
    auto elements() const -> const std::map<Element, double>&;

    /// Return the molar mass of the species (in units of kg/mol).
    auto molarMass() const -> double;

    /// Return the electrical charge of the species.
    auto charge() const -> double;

    /// Return the stoichiometry of an element in the species.
    auto elementCoefficient(std::string element) const -> double;

private:
    struct Impl;

    std::shared_ptr<Impl> pimpl;
};

/// Compare two Species instances for less than
auto operator<(const Species& lhs, const Species& rhs) -> bool;

/// Compare two Species instances for equality
auto operator==(const Species& lhs, const Species& rhs) -> bool;

} // namespace Reaktoro
