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

// File generated at Thu 10 May 2018 14:27:12

#include "MSSMatMSUSY_mAmu_two_scale_initial_guesser.hpp"
#include "MSSMatMSUSY_mAmu_two_scale_model.hpp"
#include "lowe.h"
#include "error.hpp"
#include "ew_input.hpp"
#include "wrappers.hpp"

#include <Eigen/Core>

namespace flexiblesusy {

#define DERIVEDPARAMETER(p) model->p()
#define INPUTPARAMETER(p) model->get_input().p
#define MODELPARAMETER(p) model->get_##p()
#define PHASE(p) model->get_##p()
#define LowEnergyConstant(p) Electroweak_constants::p
#define MODEL model

MSSMatMSUSY_mAmu_initial_guesser<Two_scale>::MSSMatMSUSY_mAmu_initial_guesser(
   MSSMatMSUSY_mAmu<Two_scale>* model_,
   const softsusy::QedQcd& qedqcd_,
   const MSSMatMSUSY_mAmu_low_scale_constraint<Two_scale>& low_constraint_,
   const MSSMatMSUSY_mAmu_susy_scale_constraint<Two_scale>& susy_constraint_
)
   : model(model_)
   , qedqcd(qedqcd_)
   , low_constraint(low_constraint_)
   , susy_constraint(susy_constraint_)
{
   if (!model)
      throw SetupError("MSSMatMSUSY_mAmu_initial_guesser: Error: pointer to model"
                       " MSSMatMSUSY_mAmu<Two_scale> must not be zero");
}

/**
 * Guesses the DR-bar model parameters by calling
 * guess_susy_parameters() and guess_soft_parameters() .
 */
void MSSMatMSUSY_mAmu_initial_guesser<Two_scale>::guess()
{
   guess_susy_parameters();
   guess_soft_parameters();
}

/**
 * Guesses the SUSY parameters (gauge, Yukawa couplings) at
 * \f$m_\text{top}^\text{pole}\f$ from the Standard Model gauge
 * couplings and fermion masses.  Threshold corrections are ignored.
 * The user-defined initial guess at the low-scale
 * (InitialGuessAtLowScale) is applied here:
 *
 * \code{.cpp}
   const auto TanBeta = INPUTPARAMETER(TanBeta);

   MODEL->set_vd(Re(LowEnergyConstant(vev)/Sqrt(1 + Sqr(TanBeta))));
   MODEL->set_vu(Re((TanBeta*LowEnergyConstant(vev))/Sqrt(1 + Sqr(TanBeta))));
   calculate_Yu_DRbar();
   calculate_Yd_DRbar();
   calculate_Ye_DRbar();

 * \endcode
 */
void MSSMatMSUSY_mAmu_initial_guesser<Two_scale>::guess_susy_parameters()
{
   softsusy::QedQcd leAtMt(qedqcd);
   const double mtpole = leAtMt.displayPoleMt();

   mu_guess = leAtMt.displayMass(softsusy::mUp);
   mc_guess = leAtMt.displayMass(softsusy::mCharm);
   mt_guess = model->get_thresholds() > 0 && model->get_threshold_corrections().mt > 0 ?
      leAtMt.displayMass(softsusy::mTop) - 30.0 :
      leAtMt.displayPoleMt();
   md_guess = leAtMt.displayMass(softsusy::mDown);
   ms_guess = leAtMt.displayMass(softsusy::mStrange);
   mb_guess = leAtMt.displayMass(softsusy::mBottom);
   me_guess = model->get_thresholds() > 0 ?
      leAtMt.displayMass(softsusy::mElectron) :
      leAtMt.displayPoleMel();
   mm_guess = model->get_thresholds() > 0 ?
      leAtMt.displayMass(softsusy::mMuon) :
      leAtMt.displayPoleMmuon();
   mtau_guess = leAtMt.displayMass(softsusy::mTau);

   // guess gauge couplings at mt
   const auto alpha_sm(leAtMt.guess_alpha_SM5(mtpole));

   MODEL->set_g1(Sqrt(4. * Pi * alpha_sm(0)));
   MODEL->set_g2(Sqrt(4. * Pi * alpha_sm(1)));
   MODEL->set_g3(Sqrt(4. * Pi * alpha_sm(2)));


   model->set_scale(mtpole);

   // apply user-defined initial guess at the low scale
   const auto TanBeta = INPUTPARAMETER(TanBeta);

   MODEL->set_vd(Re(LowEnergyConstant(vev)/Sqrt(1 + Sqr(TanBeta))));
   MODEL->set_vu(Re((TanBeta*LowEnergyConstant(vev))/Sqrt(1 + Sqr(TanBeta))));
   calculate_Yu_DRbar();
   calculate_Yd_DRbar();
   calculate_Ye_DRbar();

}

void MSSMatMSUSY_mAmu_initial_guesser<Two_scale>::calculate_DRbar_yukawa_couplings()
{
   calculate_Yu_DRbar();
   calculate_Yd_DRbar();
   calculate_Ye_DRbar();
}

/**
 * Calculates the Yukawa couplings Yu of the up-type quarks
 * from the Standard Model up-type quark masses (ignoring threshold
 * corrections).
 */
void MSSMatMSUSY_mAmu_initial_guesser<Two_scale>::calculate_Yu_DRbar()
{
   Eigen::Matrix<std::complex<double>,3,3> upQuarksDRbar(ZEROMATRIXCOMPLEX(3,3));
   upQuarksDRbar(0,0) = mu_guess;
   upQuarksDRbar(1,1) = mc_guess;
   upQuarksDRbar(2,2) = mt_guess;

   const auto vu = MODELPARAMETER(vu);
   MODEL->set_Yu((((1.4142135623730951*upQuarksDRbar)/vu).transpose()).real());

}

/**
 * Calculates the Yukawa couplings Yd of the down-type
 * quarks from the Standard Model down-type quark masses (ignoring
 * threshold corrections).
 */
void MSSMatMSUSY_mAmu_initial_guesser<Two_scale>::calculate_Yd_DRbar()
{
   Eigen::Matrix<std::complex<double>,3,3> downQuarksDRbar(ZEROMATRIXCOMPLEX(3,3));
   downQuarksDRbar(0,0) = md_guess;
   downQuarksDRbar(1,1) = ms_guess;
   downQuarksDRbar(2,2) = mb_guess;

   const auto vd = MODELPARAMETER(vd);
   MODEL->set_Yd((((1.4142135623730951*downQuarksDRbar)/vd).transpose()).real()
      );

}

/**
 * Calculates the Yukawa couplings Ye of the leptons
 * from the Standard Model down-type lepton masses (ignoring threshold
 * corrections).
 */
void MSSMatMSUSY_mAmu_initial_guesser<Two_scale>::calculate_Ye_DRbar()
{
   Eigen::Matrix<std::complex<double>,3,3> downLeptonsDRbar(ZEROMATRIXCOMPLEX(3,3));
   downLeptonsDRbar(0,0) = me_guess;
   downLeptonsDRbar(1,1) = mm_guess;
   downLeptonsDRbar(2,2) = mtau_guess;

   const auto vd = MODELPARAMETER(vd);
   MODEL->set_Ye((((1.4142135623730951*downLeptonsDRbar)/vd).transpose()).real(
      ));

}

/**
 * Guesses the soft-breaking parameters.  At first it runs to the
 * guess of the SUSY-scale (SUSYScaleFirstGuess) and imposes the
 * SUSY-scale constraint (SUSYScaleInput).  Afterwards, it runs to the
 * low-scale guess (LowScaleFirstGuess) and solves the EWSB conditions
 * at the tree-level.  Finally the DR-bar mass spectrum is calculated.
 */
void MSSMatMSUSY_mAmu_initial_guesser<Two_scale>::guess_soft_parameters()
{
   const double low_scale_guess = low_constraint.get_initial_scale_guess();
   const double susy_scale_guess = susy_constraint.get_initial_scale_guess();

   model->run_to(susy_scale_guess, running_precision);

   // set EWSB loop order to 0 temporarily
   const auto lo = model->get_ewsb_loop_order();
   model->set_ewsb_loop_order(0);

   // apply susy-scale constraint
   susy_constraint.set_model(model);
   susy_constraint.apply();

   model->set_ewsb_loop_order(lo);

   model->run_to(low_scale_guess, running_precision);

   // apply EWSB constraint
   model->solve_ewsb_tree_level();

   // calculate tree-level spectrum
   model->calculate_DRbar_masses();
}

} // namespace flexiblesusy
