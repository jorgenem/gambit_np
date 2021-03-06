// ====================================================================
// This file is part of FlexibleSUSY.
//
// FlexibleSUSY is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// FlexibleSUSY is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FlexibleSUSY.  If not, see
// <http://www.gnu.org/licenses/>.
// ====================================================================

#include "wrappers.hpp"

namespace flexiblesusy {

double MaxRelDiff(double a, double b)
{
   const double sTin = fabs(a);
   const double sTout = fabs(b);
   const double maxx = std::max(sTin, sTout);
   const double underflow = 1.0e-20;

   if (maxx < underflow)
      return 0.0;

   return std::abs((a - b) / maxx);
}

double MaxRelDiff(const std::complex<double>& a, const std::complex<double>& b)
{
   const double sTin = std::abs(a);
   const double sTout = std::abs(b);
   const double maxx = std::max(sTin, sTout);
   const double underflow = 1.0e-20;

   if (maxx < underflow)
      return 0.0;

   return std::abs(a - b) / maxx;
}

} // namespace flexiblesusy
