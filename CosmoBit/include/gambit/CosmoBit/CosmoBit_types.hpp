//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  Type definition header for module CosmoBit.
///
///  Compile-time registration of type definitions
///  required for the rest of the code to
///  communicate with CosmoBit.
///
///  Add to this if you want to define a new type
///  for the functions in CosmoBit to return, but
///  you don't expect that type to be needed by
///  any other modules.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///
///  \author Patrick Stoecker
///          (stoecker@physik.rwth-aachen.de)
///  \date 2017 Nov
///  \date 2018 May
///
///  \author Janina Renk
///          (janina.renk@fysik.su.se)
///  \date 2018 Oct
///  \date 2019 Mar
///
///  *********************************************


#ifndef __CosmoBit_types_hpp__
#define __CosmoBit_types_hpp__

#include "gambit/Backends/backend_types/class.hpp"
#include "gambit/Backends/backend_types/MontePythonLike.hpp"
#include <valarray>

#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/functional.h>

namespace Gambit
{

  namespace CosmoBit
  {

    typedef std::map< std::string,std::valarray < double > > map_str_valarray_dbl;
    
    class MPLike_data_container
    {
    /* Class holing MPLIke data structure & map with initialised Likelihoods objects; this is
        separated form the Classy_cosmo_container since it needs to be initialised as 'static const'
        such that the initialisation and reading in of data only happens once. 
        This is essential since the parsing of the data at initialisation of a Likelihood object can take
        much longer than the actual Likelihood calculation. 
        --
        Memebers:
        --
        pybind11::object data : MPLike data structure
        map_str_pyobj likelihoods : map likelihood name to initialised MPLike likelihood object
    */
    public:

        MPLike_data_container();
        MPLike_data_container(pybind11::object &data_in, map_str_pyobj likelihoods_in);
        
        pybind11::object data;
        map_str_pyobj likelihoods;

    };

    class Classy_cosmo_container
    {
    /* Class holding python object cosmo which is an instance of the class Class() from classy (Python wrapper for CLASS)
        This needs to be passed around between the classy frontend, CosmoBit & the MPLike frontend. 
    */
    public:
        Classy_cosmo_container(){}
        pybind11::object cosmo;
        pybind11::dict cosmo_input_dict;
        pybind11::dict cosmo_prec_dict;

        void set_input_dict(pybind11::dict input_dict);
    };

    class BBN_container
    {
      public:
        BBN_container();

        std::vector<double> BBN_abund;
        std::vector< std::vector<double> > BBN_covmat;
        std::map<std::string, int> abund_map;

        void init_arr(int nnuc);
        int get_NNUC(){return NNUC;}; // global parameter in AlterBBN, holds number of computed element abundances
        std::map<std::string,int> get_map(){return abund_map;};

      private:
        int NNUC;
    };

    class SM_time_evo 
    {
     /* Time evolution of photon & neutrino temperature as well as Hubble rate in SM for time t > 1e3 s.
        Heads-up: explicit SM-specific assumptions enter. I.e.
            - Neff = 3.046 -> T_nu/T_gamma = (Neff/Nnu)^(1/4) (4/11)^(1/4) = 0.716486
            - Only contribution to Omega_rad in early Universe are photons & neutrinos
            - g_star (relativistic degrees of freedom) is constant in time 
        => Use these routines only for times t >~ 10^3 s, where electrons and positrons are 
        already completely annihilated 
        For times after CMB release you can get these values from the class background structure.
     */
      public:
        SM_time_evo(double t0, double tf, double grid_size);
       
        // SM photon temperature (keV) as a function of time (seconds)
        // T_g [keV] = T_cmb [keV] / sqrt(2*H0*(Omega_r0)^0.5   
        //           = { [2 sqrt(8pi^3 G[_SI_] (kB [J/K])^4] *g_star_SM / [90 c^2 (c hbar [SI])^3] }^-0.5 
        // with G: Newton's constant in m^3/kg/s^2, kB: Boltzmann const in J/K, c: speed of light in m/s, 
        // hbar x c: reduced Planck const in Jm 
        // H0 and T_cmb cancel out in SM where Omega_r0 = [1 + Neff] Omega_g0, the factor that stays besides natural constants is 
        // g_star = g_gamma + 7/8 * g_nu * Neff * (4/11)^(4/3) = 3.38354
        void set_T_evo() {T_evo = factor_T_evo/sqrt(t_grid);}

        // SM neutrino temperature (keV) as a function of time (seconds)
        // Defined in this way, one gets Neff = 3.046, using Nnu = 3 SM neutrinos (Tnu = 0.716486 T(t0))
        void set_Tnu_evo() {Tnu_evo = factor_Tnu_evo/sqrt(t_grid);}
        
        // SM Hubble rate (1/s) as a function of time (seconds)
        // Solve Friedmann eq. for rad. dominated Universe -> H(t) = (da/dt)/a = H0 sqrt(Omega_g0 a^-4)
        void set_Ht_evo() {H_evo= 1./(2.0*t_grid);} 
        
        // SM Hubble rate (1/s) as a function of temperature (keV)
        // H^2 = 8 pi G/3 rho = 8 pi G/3  g_star pi^2/30 (k_b T)^4/(hbar c)^3
        void set_HT_evo(std::valarray<double> T) {H_evo = factor_HT_evo*T*T;}

        // fast integration of Hubble rate using trapezoidal integration
        void calc_H_int();

        std::valarray<double> get_t_grid() const {return t_grid;}
        std::valarray<double> get_T_evo() const {return T_evo;}
        std::valarray<double> get_Tnu_evo() const {return Tnu_evo;}
        std::valarray<double> get_H_evo() const {return H_evo;}
        std::valarray<double> get_H_int() const {return H_int;}

    private:
        int grid_size;
        std::valarray<double> t_grid;
        std::valarray<double> T_evo;
        std::valarray<double> Tnu_evo;
        std::valarray<double> H_evo;
        std::valarray<double> H_int;

        double factor_T_evo;
        double factor_Tnu_evo;
        double factor_HT_evo;
    };


    // Forward declaration of warnings and errors
    error& CosmoBit_error();
    warning& CosmoBit_warning();

    // map (dictionary) for name and value of the inputs for Class
    class ClassInput
    {
      public:
        //classInput();
        //~classInput();
        void addEntry(std::string key,std::string val);
        void addEntry(std::string key,double val);
        void addEntry(std::string key,int val);
        void addEntry(std::string key,std::vector<double> val);
        void addEntry(std::string key,std::vector<int> val);
        std::string print_entries_to_logger();

        void clear();
        std::map<std::string,std::string> get_map();

      private:
        std::map<std::string,std::string> input_list;
    };
    
    // Container for the structs of Class
    class Class_container
    {
      public:
        Class_container();
        //~Class_container();

        ClassInput input;

        int lmax;
        std::vector<double> Cl_TT;
        std::vector<double> Cl_TE;
        std::vector<double> Cl_EE;
        std::vector<double> Cl_BB;
        std::vector<double> Cl_PhiPhi;

        std::vector<double> Pk_S; // Primordial Scalar Power Spectrum
        std::vector<double> Pk_T; // Primordial Tensor Power Spectrum
        std::vector<double> k_ar; // Corresponding wavenumbers.

        std::string backend_in_use;
    };


/*
    ----------  ClassyInput Methods ---------
    In the following the methods of the Class 'ClassyInput' are implemented. 
    ClassyInput has the attribute 'input_dict' which is a python dictionary 
    containing the input parameters for CLASS

*/
    // Class that manages the input dictionary for classy
    class ClassyInput
    {
      public:
        // add all entries from extra_entries to input_dict, will throw an error if 
        // one entry is contained in both dictionaries
        // returns 1 if adding was successful, -1 if an entries 
        // appeared twice -> need to throw an error since overwriting CLASS input
        // without realising it can be dangerous
        int addDict(pybind11::dict extra_entries);

        void addEntry(str key,double value){input_dict[key.c_str()]=value;};
        void addEntry(str key,int value){input_dict[key.c_str()]=value;};
        //void addEntry(str key,int value){input_dict[key.c_str()]=value;};
        void addEntry(str key,str value){input_dict[key.c_str()]=value.c_str();};

        bool hasKey(str key){return input_dict.contains(key.c_str());};
        //int addEntry(str key,std::ostringstream value){input_dict[key.c_str()]=value.c_str()};
        
        // add all entries from extra_entries to input_dict, will overwrite any value
        // contained in input_dict if 
        // we actually should not allow to overwrite .. 
        //void addEntries_and_overwrite(pybind11::dict extra_entries);

        // merge 
        void merge_input_dicts(pybind11::dict extra_entries);
        
        // routine to print CLASS input values to logger
        std::string print_entries_to_logger();

        // clears all entries from input_dict
        void clear();

        // return input_dict
        pybind11::dict get_input_dict();

      private:
        pybind11::dict input_dict;
    };
  }
}

#endif // defined __CosmoBit_types_hpp__
