//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  Frontend header for the MultiModeCode backend.
///
///  Compile-time registration of available
///  functions and variables from this backend.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///
///  \author Selim C. Hotinli
///          (selim.hotinli14@pimperial.ac.uk)
///  \date 2017 Jul
///
///  *********************************************

#define BACKENDNAME MultiModeCode
#define BACKENDLANG FORTRAN
#define VERSION 2.0.0
#define SAFE_VERSION 2_0_0

// Load it
LOAD_LIBRARY

BE_FUNCTION(multimodecode_gambit_driver, gambit_inflation_observables, (int& ,int& ,  int& ,  int& ,int& ,  int& ,  int& ,  int& ,  int& ,int& ,double& ,int& ,  int& ,double& ,int& ,double*,double*,  int& ,  int& ,double*,double*,double*,double& ,double& ,double& ,  int& ,int&, double&, double& ,double*,double*,double*,double*,double& ,double&),"__multimodecode_gambit_MOD_multimodecode_gambit_driver","multimodecode_gambit_driver")

BE_FUNCTION(multimodecode_gambit_driver_test, void,(gambit_inflation_observables*, int& , int& ,   int& ,  int& ,int& ,  int& ,  int& ,  int& ,   int& ,int& ,double& ,int& ,  int& ,double& ,int& ,  int& ,  int& ,double& ,double& ,double& ,  int& ,int& ,double& ,double& ,double&),"__multimodecode_gambit_MOD_multimodecode_gambit_driver","multimodecode_gambit_driver_test")


namespace Gambit
{
	namespace Backends
	{
		namespace CAT_3(BACKENDNAME,_,SAFE_VERSION)
		{
			
			/* Convenience functions go here */
			
		} /* end namespace BACKENDNAME_SAFE_VERSION */
	} /* end namespace Backends */
} /* end namespace Gambit */


//BE_CONV_FUNCTION(awesomenessByAnders, double, "awesomeness")

BE_INI_FUNCTION{}
END_BE_INI_FUNCTION

// Undefine macros to avoid conflict with other backends
#include "gambit/Backends/backend_undefs.hpp"
