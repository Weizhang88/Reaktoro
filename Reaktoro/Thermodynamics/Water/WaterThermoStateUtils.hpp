// Reaktoro is a C++ library for computational reaction modelling.
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

#pragma once

namespace Reaktoro {

// Forward declarations
struct WaterThermoState;
struct WaterHelmholtzState;

/// Calculate the thermodynamic state of water using the Haar-Gallagher-Kell (1984) equation of state
/// @param T The temperature of water (in units of K)
/// @param P The pressure of water (in units of Pa)
/// @return The thermodynamic state of water
/// @see WaterThermoState
auto waterThermoStateHGK(double T, double P) -> WaterThermoState;

/// Calculate the thermodynamic state of water using the Wagner and Pruss (1995) equation of state
/// @param T The temperature of water (in units of K)
/// @param P The pressure of water (in units of Pa)
/// @return The thermodynamic state of water
/// @see WaterThermoState
auto waterThermoStateWagnerPruss(double T, double P) -> WaterThermoState;

/// Calculate the thermodynamic state of water.
/// This is a general method that uses the Helmholtz free energy state
/// of water, as an instance of WaterHelmholtzState, to completely
/// resolve its thermodynamic state.
/// @param T The temperature of water (in units of K)
/// @param P The pressure of water (in units of Pa)
/// @param D The density of water (in units of kg/m3)
/// @param wh The Helmholtz free energy state of water
/// @return The thermodynamic state of water
/// @see WaterHelmholtzState, WaterThermoState
auto waterThermoState(double T, double P, double D, const WaterHelmholtzState& wh) -> WaterThermoState;

} // namespace Reaktoro