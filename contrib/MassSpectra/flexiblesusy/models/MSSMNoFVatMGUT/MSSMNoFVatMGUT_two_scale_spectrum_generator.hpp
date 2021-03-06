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

// File generated at Thu 10 May 2018 14:52:01

#ifndef MSSMNoFVatMGUT_TWO_SCALE_SPECTRUM_GENERATOR_H
#define MSSMNoFVatMGUT_TWO_SCALE_SPECTRUM_GENERATOR_H

#include "MSSMNoFVatMGUT_spectrum_generator_interface.hpp"
#include "MSSMNoFVatMGUT_spectrum_generator.hpp"
#include "MSSMNoFVatMGUT_two_scale_model.hpp"
#include "MSSMNoFVatMGUT_model_slha.hpp"

namespace softsusy { class QedQcd; }

namespace flexiblesusy {

class Two_scale;

template <>
class MSSMNoFVatMGUT_spectrum_generator<Two_scale>
   : public MSSMNoFVatMGUT_spectrum_generator_interface<Two_scale> {
public:
   MSSMNoFVatMGUT_spectrum_generator() = default;
   virtual ~MSSMNoFVatMGUT_spectrum_generator() = default;

   double get_high_scale() const { return high_scale; }
   double get_susy_scale() const { return susy_scale; }
   double get_low_scale()  const { return low_scale;  }
   double get_pole_mass_scale() const;

   void write_running_couplings(const std::string& filename = "MSSMNoFVatMGUT_rgflow.dat") const;

protected:
   virtual void run_except(const softsusy::QedQcd&, const MSSMNoFVatMGUT_input_parameters&) override;

private:
   double high_scale{0.};
   double susy_scale{0.};
   double low_scale{0.};

   void calculate_spectrum();
};

} // namespace flexiblesusy

#endif
