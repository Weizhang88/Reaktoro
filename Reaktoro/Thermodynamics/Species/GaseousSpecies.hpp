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
#include <string>

// Reaktoro includes
#include <Reaktoro/Core/Species.hpp>
#include <Reaktoro/Thermodynamics/Species/ThermoData.hpp>

namespace Reaktoro {

/// A type to describe the attributes of a gaseous species
class GaseousSpecies : public Species
{
public:
    /// Construct a default GaseousSpecies instance
    GaseousSpecies();

    /// Construct an GaseousSpecies instance from a Species instance
    GaseousSpecies(const Species& species);

    /// Set the critical temperature of the gaseous species (in units of K)
    auto setCriticalTemperature(double val) -> void;

    /// Set the critical pressure of the gaseous species (in units of Pa)
    auto setCriticalPressure(double val) -> void;

    /// Set the acentric factor of the gaseous species
    auto setAcentricFactor(double val) -> void;

    /// Set the thermodynamic data of the gaseous species.
    auto setThermoData(const GaseousSpeciesThermoData& thermo) -> void;

    /// Return the critical temperature of the gaseous species (in units of K)
    auto criticalTemperature() const -> double;

    /// Return the critical pressure of the gaseous species (in units of Pa)
    auto criticalPressure() const -> double;

    /// Return the acentric factor of the gaseous species
    auto acentricFactor() const -> double;

    /// Return the thermodynamic data of the gaseous species.
    auto thermoData() const -> const GaseousSpeciesThermoData&;

private:
    struct Impl;

    std::shared_ptr<Impl> pimpl;
};

} // namespace Reaktoro
