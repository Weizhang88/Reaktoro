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

#include "ThermoScalar.hpp"

// Reaktoro includes
#include <Reaktoro/Common/ThermoVector.hpp>

namespace Reaktoro {

ThermoScalar::ThermoScalar()
{}

ThermoScalar::ThermoScalar(double val, double ddt, double ddp)
: val(val), ddt(ddt), ddp(ddp)
{}

auto ThermoScalar::operator=(const ThermoVectorRow& row) -> ThermoScalar&
{
    val = row.val;
    ddt = row.ddt;
    ddp = row.ddp;
    return *this;
}

auto ThermoScalar::operator=(const ThermoVectorConstRow& row) -> ThermoScalar&
{
    val = row.val;
    ddt = row.ddt;
    ddp = row.ddp;
    return *this;
}

auto ThermoScalar::operator+=(const ThermoScalar& other) -> ThermoScalar&
{
    val += other.val;
    ddt += other.ddt;
    ddp += other.ddp;
    return *this;
}

auto ThermoScalar::operator-=(const ThermoScalar& other) -> ThermoScalar&
{
    val -= other.val;
    ddt -= other.ddt;
    ddp -= other.ddp;
    return *this;
}

auto ThermoScalar::operator*=(double scalar) -> ThermoScalar&
{
    val *= scalar;
    ddt *= scalar;
    ddp *= scalar;
    return *this;
}

auto ThermoScalar::operator/=(double scalar) -> ThermoScalar&
{
    *this *= 1.0/scalar;
    return *this;
}

auto operator==(const ThermoScalar& l, const ThermoScalar& r) -> bool
{
    return l.val == r.val and
           l.ddt == r.ddt and
           l.ddp == r.ddp;
}

auto operator+(const ThermoScalar& l) -> ThermoScalar
{
    return l;
}

auto operator-(const ThermoScalar& l) -> ThermoScalar
{
    return -1.0 * l;
}

auto operator+(const ThermoScalar& l, const ThermoScalar& r) -> ThermoScalar
{
    ThermoScalar res = l;
    res += r;
    return res;
}

auto operator-(const ThermoScalar& l, const ThermoScalar& r) -> ThermoScalar
{
    ThermoScalar res = l;
    res -= r;
    return res;
}

auto operator*(double scalar, const ThermoScalar& r) -> ThermoScalar
{
    ThermoScalar res = r;
    res *= scalar;
    return res;
}

auto operator*(const ThermoScalar& l, double scalar) -> ThermoScalar
{
    return scalar * l;
}

auto operator*(const ThermoScalar& l, const ThermoScalar& r) -> ThermoScalar
{
    ThermoScalar res;
    res.val = l.val * r.val;
    res.ddt = l.ddt * r.val + l.val * r.ddt;
    res.ddp = l.ddp * r.val + l.val * r.ddp;
    return res;
}

auto operator/(double scalar, const ThermoScalar& r) -> ThermoScalar
{
    const double factor = -scalar/(r.val * r.val);
    ThermoScalar res;
    res.val = scalar/r.val;
    res.ddt = factor * r.ddt;
    res.ddp = factor * r.ddp;
    return res;
}

auto operator/(const ThermoScalar& l, double scalar) -> ThermoScalar
{
    return (1.0/scalar) * l;
}

auto operator/(const ThermoScalar& l, const ThermoScalar& r) -> ThermoScalar
{
    const double factor = 1.0/(r.val * r.val);
    ThermoScalar res;
    res.val = l.val / r.val;
    res.ddt = (r.val * l.ddt - l.val * r.ddt) * factor;
    res.ddp = (r.val * l.ddp - l.val * r.ddp) * factor;
    return res;
}

auto pow(const ThermoScalar& a, double power) -> ThermoScalar
{
    ThermoScalar b;
    b.val = std::pow(a.val, power);
    b.ddt = power * b.val * a.ddt/a.val;
    b.ddp = power * b.val * a.ddp/a.val;
    return b;
}

auto exp(const ThermoScalar& a) -> ThermoScalar
{
    ThermoScalar b;
    b.val = std::exp(a.val);
    b.ddt = b.val * a.ddt;
    b.ddp = b.val * a.ddp;
    return b;
}

auto log(const ThermoScalar& a) -> ThermoScalar
{
    const double factor = 1.0/a.val;
    ThermoScalar b;
    b.val = std::log(a.val);
    b.ddt = factor * a.ddt;
    b.ddp = factor * a.ddp;
    return b;
}

auto log10(const ThermoScalar& a) -> ThermoScalar
{
    const double ln10 = 2.30258509299;
    const double factor = 1.0/(ln10 * a.val);
    ThermoScalar b;
    b.val = std::log10(a.val);
    b.ddt = factor * a.ddt;
    b.ddp = factor * a.ddp;
    return b;
}

} // namespace Reaktoro