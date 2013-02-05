//  GAMBIT: Global and Modular BSM Inference Tool
//  *********************************************
//
//  Functions of module ExampleBit_B
//
//  Put your functions in files like this
//  if you wish to add observables or likelihoods
//  to this module.
//
//  *********************************************
//
//  Authors
//  =======
//
//  (add name and date if you modify)
//
//  Pat Scott
//  2012 Nov 15 
//  2013 Jan 18, Feb 04
//
//  Christoph Weniger
//  Jan 17 2013
//
//  *********************************************

#include <string>
#include <iostream>

namespace GAMBIT {

  namespace ExampleBit_B {

    // Initialization routine
    void initialize () {
      std::cout << std::endl;
      std::cout << "********************************************" << std::endl;
      std::cout << "***       Initializing ExampleBit_B      ***" << std::endl;
      std::cout << "********************************************" << std::endl;
    }

    // Module functions
    void xsection (double &result)               { result = 5.e10; }
    void nevents_postcuts (int &result)          { result = 1; }
    void authors_dogs_name (std::string &result) { result = "ImaginaryPuppy"; }
    void nevents (int &result)                   { result = 2; }

  }

}

