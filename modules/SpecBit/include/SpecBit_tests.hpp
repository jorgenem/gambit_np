//   GAMBIT: Global and Modular BSM Inference Tool
//   *********************************************
///  \file
///
///  Test functions for Spectrum object
///
///  These functions test various aspects of the
///  Spectrum class and derived classes. They are
///  not dependent on any other parts of Gambit,
///  so that Spectrum object test code can be
///  compiled and run seperately from Gambit, but
///  but also be run from inside Gambit.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///
///  \author Ben Farmer
///          (ben.farmer@gmail.com)
///    \date 2014 Dec
///  
///  *********************************************

// Flexible SUSY stuff (should not be needed by the rest of gambit)
#include "CMSSM_two_scale_model.hpp"
#include "CMSSM_two_scale_model_slha.hpp"
//#include "CMSSM_physical.hpp"

#include "ew_input.hpp"
#include "MSSMSpec.hpp"
#include "numerics.hpp"

// Switch test output depending on where this is being compiled
#ifdef IN_SPECBIT
  #define OUTPUT logger()
  #define TAGerr LogTags::err
  #define TAGfatal LogTags::fatal
  #define TAGeom EOM
#else
  #define OUTPUT std::cerr
  #define TAGerr "" 
  #define TAGfatal ""
  #define TAGeom ""
#endif



namespace Gambit
{
   
   namespace SpecBit
   {
      //using namespace LogTags;
      using namespace flexiblesusy;

      bool print_error(bool pass, std::string get_type, std::string data, 
                       double sting_get_out, double data_member,  
                       int i = -1, int j = -1) { 
        
         OUTPUT << " returning fail on test for: " << std::endl;
         if (i > -1) OUTPUT << "with first index = " << i << std::endl;
         if (j > -1) OUTPUT << "with second index = " << j << std::endl;
         OUTPUT << get_type << " with " << data << " string arg."  <<std::endl; 
         OUTPUT << "string getter gives = " 
                <<sting_get_out  << std::endl;
         OUTPUT << "data member is = "  
                << data_member  << std::endl;
         OUTPUT << TAGerr << TAGfatal << TAGeom;
         return pass;
         
      }

      void print_error(std::string get_type, std::string name, 
                       double sting_get_out, double data_member,  
                       int i = -1, int j = -1) { 

         OUTPUT << " returning fail on test for: " << std::endl;
         if (i > -1) OUTPUT << "with first index = " << i << std::endl;
         if (j > -1) OUTPUT << "with second index = " << j << std::endl;
         OUTPUT << get_type << " with " << name << " string arg."  
                <<std::endl; 
         OUTPUT << "string getter gives = " <<sting_get_out  << std::endl;
         OUTPUT << "data member is = "  
                << data_member  << std::endl;
         OUTPUT << TAGerr << TAGfatal << TAGeom;
      }

      bool test_getters(std::string get_type, 
                        std::string name, double getter_output, 
                        double data_member, int i = -1, int j = -1){
         bool pass = is_equal(getter_output,data_member);
         // if(pass == false) return pass;
         if(pass == false)
            print_error(get_type, name, getter_output, data_member,i,j); 
         return pass;
      }

      
      /// Module convenience functions
      // These are not known to Gambit
    
      template <class M>
      bool TestMssmParMass2_0(MSSMSpec<M> mssm, M FSmssm, 
                              bool immediate_exit = true){      
         std::string name = "BMu";
         bool pass = test_getters("get_mass2_parameter", name,  
                          mssm.mssm_drbar_pars.get_mass2_parameter(name),  
                           FSmssm.get_BMu());
         if(immediate_exit == true && pass == false) return pass; 
       
         //do all in loop 
         std::set<std::pair<std::string,double>> name_value = {
            {"BMu", FSmssm.get_BMu()},
            {"mHd2", FSmssm.get_mHd2()},
            {"mHu2", FSmssm.get_mHu2()} 
         };
          
          std::set<std::pair<std::string, double>>::iterator iter;
       for(iter=name_value.begin(); iter != name_value.end(); ++iter)
          {
             pass = test_getters("get_mass2_parameter", iter->first,  
                    mssm.mssm_drbar_pars.get_mass2_parameter(iter->first), 
                                 iter->second);
       if(immediate_exit == true && pass == false) return pass; 
          }
    
       //repeat for older type of getters
       pass =  is_equal(mssm.mssm_drbar_pars.get_mass2_par("BMu"),
                        FSmssm.get_BMu());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_mass2_par("mHd2"),
                       FSmssm.get_mHd2());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_mass2_par("mHu2"),
                       FSmssm.get_mHu2());
                            
                            
       return pass;
    }
     
    template <class M>  
    bool TestMssmParMass2_2(MSSMSpec<M> mssm, M FSmssm, 
                           bool immediate_exit=true){
      bool pass = false;
     
      for(int i=1; i<=3; i++){
         for(int j=1; j<=3; j++){
            //Would be smarter to take these out of for loop and
            // use function pointers, but I won't.
            std::set<std::pair<std::string,double>> name_value = {
               {"mq2", FSmssm.get_mq2(i-1,j-1)},
               {"mu2", FSmssm.get_mu2(i-1,j-1)},
               {"md2", FSmssm.get_md2(i-1,j-1)},
               {"ml2", FSmssm.get_ml2(i-1,j-1)},
               {"me2", FSmssm.get_me2(i-1,j-1)}               
            };     
          
            

            std::set<std::pair<std::string, double>>::iterator iter;
       for(iter=name_value.begin(); iter != name_value.end(); ++iter)
          {
             pass = test_getters("get_mass2_parameter", iter->first,  
                                 mssm.mssm_drbar_pars.get_mass2_parameter(iter->first,i,j), 
                                 iter->second, i, j);
       if(immediate_exit == true && pass == false) return pass; 
          }

            pass = is_equal(mssm.mssm_drbar_pars.get_mass2_parameter("mq2",i,j),
                            FSmssm.get_mq2(i-1,j-1)); 
            if(pass == false) return pass;
            pass = is_equal(mssm.mssm_drbar_pars.get_mass2_parameter("ml2",i,j),
                            FSmssm.get_ml2(i-1,j-1)); 
            if(pass == false) return pass;
            pass = is_equal(mssm.mssm_drbar_pars.get_mass2_parameter("md2",i,j),
                            FSmssm.get_md2(i-1,j-1)); 
            if(pass == false) return pass;
            pass = is_equal(mssm.mssm_drbar_pars.get_mass2_parameter("mu2",i,j),
                             FSmssm.get_mu2(i-1,j-1)); 
            if(pass == false) return pass;
            pass = is_equal(mssm.mssm_drbar_pars.get_mass2_parameter("me2",i,j),
                            FSmssm.get_me2(i-1,j-1)); 
            if(pass == false) return pass;
             
               
             
            //repeat for older type of getters
            pass = is_equal(mssm.mssm_drbar_pars.get_mass2_par("mq2",i-1,j-1),
                            FSmssm.get_mq2(i-1,j-1)); 
            if(pass == false) return pass;
            pass = is_equal(mssm.mssm_drbar_pars.get_mass2_par("ml2",i-1,j-1),
                            FSmssm.get_ml2(i-1,j-1)); 
            if(pass == false) return pass;
            pass = is_equal(mssm.mssm_drbar_pars.get_mass2_par("md2",i-1,j-1),
                            FSmssm.get_md2(i-1,j-1)); 
            if(pass == false) return pass;
            pass = is_equal(mssm.mssm_drbar_pars.get_mass2_par("mu2",i-1,j-1),
                            FSmssm.get_mu2(i-1,j-1)); 
            if(pass == false) return pass;
            pass = is_equal(mssm.mssm_drbar_pars.get_mass2_par("me2",i-1,j-1),
                            FSmssm.get_me2(i-1,j-1)); 
            if(pass == false) return pass;

              
         }
       }
       return pass;
    }
    
    template <class M>  
    bool TestMssmParMass1_0(MSSMSpec<M> mssm, M FSmssm){
       //we test both 
       bool pass = is_equal(mssm.mssm_drbar_pars.get_mass_parameter("M1"),
                            FSmssm.get_MassB());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_mass_parameter("M2"),
                       FSmssm.get_MassWB());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_mass_parameter("M3"),
                       FSmssm.get_MassG());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_mass_parameter("Mu"),
                       FSmssm.get_Mu());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_mass_parameter("vu"),
                       FSmssm.get_vu());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_mass_parameter("vd"),
                       FSmssm.get_vd());
       if(pass == false) return pass;
       // Now versions using old getters
       pass =  is_equal(mssm.mssm_drbar_pars.get_mass_par("M1"),
                        FSmssm.get_MassB());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_mass_par("M2"),
                       FSmssm.get_MassWB());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_mass_par("M3"),
                       FSmssm.get_MassG());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_mass_par("Mu"),FSmssm.get_Mu());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_mass_par("vu"),FSmssm.get_vu());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_mass_par("vd"),FSmssm.get_vd());
       if(pass == false) return pass;
       return pass;
    }
    
    template <class M>  
    bool TestMssmParMass1_2(MSSMSpec<M> mssm, M FSmssm){
       bool pass = false;
       for(int i=1; i<=3; i++){
          for(int j=1; j<=3; j++){
             pass = is_equal(mssm.mssm_drbar_pars.get_mass_parameter("TYd",i,j),
                             FSmssm.get_TYd(i-1,j-1)); 
             if(pass == false) return pass;
             pass = is_equal(mssm.mssm_drbar_pars.get_mass_parameter("TYe",i,j),
                             FSmssm.get_TYe(i-1,j-1)); 
             if(pass == false) return pass;
             pass = is_equal(mssm.mssm_drbar_pars.get_mass_parameter("TYu",i,j),
                             FSmssm.get_TYu(i-1,j-1)); 
             if(pass == false) return pass;
            
             
             //repeat for older type of getters
             pass = is_equal(mssm.mssm_drbar_pars.get_mass_par("TYd",i-1,j-1),
                             FSmssm.get_TYd(i-1,j-1)); 
             if(pass == false) return pass;
             pass = is_equal(mssm.mssm_drbar_pars.get_mass_par("TYe",i-1,j-1),
                             FSmssm.get_TYe(i-1,j-1)); 
             if(pass == false) return pass;
             pass = is_equal(mssm.mssm_drbar_pars.get_mass_par("TYu",i-1,j-1),
                             FSmssm.get_TYu(i-1,j-1)); 
             if(pass == false) return pass;
          }
       }
       return pass;
    }
    
    template <class M>
    bool TestMssmParMass0_0(MSSMSpec<M> mssm, M FSmssm){
       //we test both 
       bool pass = is_equal(mssm.mssm_drbar_pars.get_dimensionless_parameter("g1"),FSmssm.get_g1());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_dimensionless_parameter("g2"),FSmssm.get_g2());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_dimensionless_parameter("g3"),FSmssm.get_g3());
       if(pass == false) return pass;
       
       // Now versions using old getters
       pass = is_equal(mssm.mssm_drbar_pars.get_dimensionless_par("g1"),FSmssm.get_g1());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_dimensionless_par("g2"),FSmssm.get_g2());
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_drbar_pars.get_dimensionless_par("g3"),FSmssm.get_g3());
       if(pass == false) return pass;
       return pass;
    }
    
    template <class M>
    bool TestMssmParMass0_2(MSSMSpec<M> mssm, M FSmssm){
       bool pass = false;
       for(int i=1; i<=3; i++){
          for(int j=1; j<=3; j++){
             pass = is_equal(mssm.mssm_drbar_pars.get_dimensionless_parameter("Yd",i,j),
                             FSmssm.get_Yd(i-1,j-1)); 
             if(pass == false) {
                return pass;
             }
                pass = is_equal(mssm.mssm_drbar_pars.get_dimensionless_parameter("Ye",i,j),
                             FSmssm.get_Ye(i-1,j-1)); 
             if(pass == false) return pass;
             pass = is_equal(mssm.mssm_drbar_pars.get_dimensionless_parameter("Yu",i,j),
                             FSmssm.get_Yu(i-1,j-1)); 
             if(pass == false) return pass;
            
             
             //repeat for older type of getters
             pass = is_equal(mssm.mssm_drbar_pars.get_dimensionless_par("Yd",i-1,j-1),
                             FSmssm.get_Yd(i-1,j-1)); 
             if(pass == false) return pass;
             pass = is_equal(mssm.mssm_drbar_pars.get_dimensionless_par("Ye",i-1,j-1),
                             FSmssm.get_Ye(i-1,j-1)); 
             if(pass == false) return pass;
             pass = is_equal(mssm.mssm_drbar_pars.get_dimensionless_par("Yu",i-1,j-1),
                             FSmssm.get_Yu(i-1,j-1)); 
             if(pass == false) return pass;
          }
       }
       return pass;
    }
    
    template <class M>
    bool TestMssmPoleGets0(MSSMSpec<M> mssm, M FSmssm){
       bool pass = false;
       pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MZ"),FSmssm.get_physical().MVZ);
       if(pass == false) return pass;
       pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MW"),FSmssm.get_physical().MVWm);
       if(pass == false) return pass;
     
       // pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MGoldstone0"),FSmssm.get_physical().MAh(0));
       // if(pass == false) return pass;
       //  pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MA0"),FSmssm.get_physical().MAh(1));
       // if(pass == false) return pass;
       // pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MGoldstonePM"),FSmssm.get_physical().MHpm(0));
       // if(pass == false) return pass;
       // pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MHpm"),FSmssm.get_physical().MHpm(1));
       // if(pass == false) return pass;
       pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MGluino"),FSmssm.get_physical().MGlu);
       if(pass == false) 
          {
             OUTPUT << " returning fail on test for: " << std::endl;
             OUTPUT << "get_Pole_Mass with MGluino string"  << std::endl; 
             OUTPUT << "mssm.mssm_ph.get_Pole_Mass(MGluino) = " 
                       << mssm.mssm_ph.get_Pole_Mass("MGluino") << std::endl;
             OUTPUT << "FSmssm.get_physical().MGlu = "  
                       << FSmssm.get_physical().MGlu << std::endl;
             OUTPUT << TAGerr << TAGfatal << TAGeom;
             return pass;
          }
       pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MGluon"),FSmssm.get_physical().MVG);
       if(pass == false) 
          {
             OUTPUT << " returning fail on test for: " << std::endl;
             OUTPUT << "get_Pole_Mass with MGluon string"  << std::endl;
             OUTPUT << TAGerr << TAGfatal << TAGeom;
             return pass;
          }
       pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MPhoton"),FSmssm.get_physical().MVP);
       if(pass == false) 
          {
             OUTPUT << " returning fail on test for: " << std::endl;
             OUTPUT << "get_Pole_Mass with MPhoton string"  << std::endl;
             OUTPUT << TAGerr << TAGfatal << TAGeom;
             return pass;
          }
       // pass = is_equal(mssm.mssm_ph.get_Pole_Mass("Mtop"),FSmssm.get_physical().MFu(2));
       // if(pass == false) 
       //    {
       //       OUTPUT << " returning fail on test for: " << std::endl;
       //       OUTPUT << "get_Pole_Mass with Mtop string"  << std::endl; 
       //       return pass;
       //    }
       // pass = is_equal(mssm.mssm_ph.get_Pole_Mass("Mcharm"),FSmssm.get_physical().MFu(1));
       //  if(pass == false) 
       //    {
       //       OUTPUT << " returning fail on test for: " << std::endl;
       //       OUTPUT << "get_Pole_Mass with Mcharm string"  << std::endl; 
       //       return pass;
       //    }
       // pass = is_equal(mssm.mssm_ph.get_Pole_Mass("Mup"),FSmssm.get_physical().MFu(0));
       //  if(pass == false) 
       //    {
       //       OUTPUT << " returning fail on test for: " << std::endl;
       //       OUTPUT << "get_Pole_Mass with Mup string"  << std::endl; 
       //       return pass;
       //    }
       // pass = is_equal(mssm.mssm_ph.get_Pole_Mass("Mbottom"),FSmssm.get_physical().MFd(2));
       //  if(pass == false) 
       //    {
       //       OUTPUT << " returning fail on test for: " << std::endl;
       //       OUTPUT << "get_Pole_Mass with Mbottom string"  << std::endl; 
       //       return pass;
       //    }
       // pass = is_equal(mssm.mssm_ph.get_Pole_Mass("Mstrange"),FSmssm.get_physical().MFd(1));
       // if(pass == false) 
       //    {
       //       OUTPUT << " returning fail on test for: " << std::endl;
       //       OUTPUT << "get_Pole_Mass with Mstrange string"  << std::endl; 
       //       return pass;
       //    }
       // pass = is_equal(mssm.mssm_ph.get_Pole_Mass("Mdown"),FSmssm.get_physical().MFd(0));
       // if(pass == false) 
       //    {
       //       OUTPUT << " returning fail on test for: " << std::endl;
       //       OUTPUT << "get_Pole_Mass with Mdown string"  << std::endl; 
       //       return pass;
       //    }
       // pass = is_equal(mssm.mssm_ph.get_Pole_Mass("Mtau"),FSmssm.get_physical().MFe(2));
       // if(pass == false) 
       //    {
       //       OUTPUT << " returning fail on test for: " << std::endl;
       //       OUTPUT << "get_Pole_Mass with Mtau string"  << std::endl; 
       //       return pass;
       //    }
       // pass = is_equal(mssm.mssm_ph.get_Pole_Mass("Mmuon"),FSmssm.get_physical().MFe(1));
       // if(pass == false) 
       //    {
       //       OUTPUT << " returning fail on test for: " << std::endl;
       //       OUTPUT << "get_Pole_Mass with Mmuon string"  << std::endl; 
       //       return pass;
       //    }
       // pass = is_equal(mssm.mssm_ph.get_Pole_Mass("Melectron"),FSmssm.get_physical().MFe(0));
       //  if(pass == false) 
       //    {
       //       OUTPUT << " returning fail on test for: " << std::endl;
       //       OUTPUT << "get_Pole_Mass with Melectron string"  << std::endl; 
       //       return pass;
       //    }
      
       return pass;
    }
   
    template <class M>
    bool TestMssmPoleGets1(MSSMSpec<M> mssm, M FSmssm){
       bool pass = false;
     
       for(int i=1; i<=6; i++){
          
          pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MSd",i),
                          FSmssm.get_physical().MSd(i-1)); 
        
          if(pass == false) return pass;
          pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MSu",i),
                          FSmssm.get_physical().MSu(i-1));
          if(pass == false) return pass;
          pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MSe",i),
                          FSmssm.get_physical().MSe(i-1));
          if(pass == false) return pass;
          
       }
       for(int i=1; i<=3; i++){
        pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MSv",i),
                          FSmssm.get_physical().MSv(i-1));
          if(pass == false) return pass;
       }
       for(int i=1; i<=2; i++){
          pass = is_equal(mssm.mssm_ph.get_Pole_Mass("Mh0",i),
                          FSmssm.get_physical().Mhh(i-1)); 
          if(pass == false){
             OUTPUT << "returning after test fail on Mh0 getter with index " 
                    << i << std::endl;
             OUTPUT << "string getter gives " 
                    << mssm.mssm_ph.get_Pole_Mass("Mh0",i) << std::endl;
             OUTPUT << "struct in model has " 
                    << FSmssm.get_physical().Mhh(i-1) << std::endl;
             OUTPUT << TAGerr << TAGfatal << TAGeom;
             return pass;
          }
       }
       //In the the neutralino and chargino tests I compare against 
       // value in physical_slha struct since the value in
       // physical may differ by a sign since it stores positive masses
       // and a complex mixing matrix.
       for(int i=1; i<=4; i++){
          pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MChi",i),
                          FSmssm.get_physical_slha().MChi(i-1)); 
          if(pass == false){
             OUTPUT << "returning after test fail on MChi getter with index " 
                    << i << std::endl;
             OUTPUT << "string getter gives " 
                    << mssm.mssm_ph.get_Pole_Mass("MChi",i) << std::endl;
             OUTPUT << "struct in model has " 
                    << FSmssm.get_physical_slha().MChi(i-1) << std::endl;
             OUTPUT << TAGerr << TAGfatal << TAGeom;
             return pass;
          }
       }
        for(int i=1; i<=2; i++){
          pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MCha",i),
                          FSmssm.get_physical_slha().MCha(i-1));   
          if(pass == false){
             OUTPUT << "returning after test fail on MCha getter with index " 
                    << i << std::endl;
             OUTPUT << "string getter gives " 
                    << mssm.mssm_ph.get_Pole_Mass("MCha",i) << std::endl;
             OUTPUT << "struct in model has " 
                    << FSmssm.get_physical_slha().MCha(i-1) << std::endl;
             OUTPUT << TAGerr << TAGfatal << TAGeom;
             return pass;
          }

        }
        for(int i=1; i<=3; i++){
          pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MFu",i),
                          FSmssm.get_physical().MFu(i-1)); 
          if(pass == false){
             OUTPUT << "returning after test fail on MFu getter with index " 
                    << i << std::endl;
             OUTPUT << "string getter gives " 
                    << mssm.mssm_ph.get_Pole_Mass("MFu",i) << std::endl;
             OUTPUT << "struct in model has " 
                    << FSmssm.get_physical().MFu(i-1) << std::endl;
             OUTPUT << TAGerr << TAGfatal << TAGeom;
             return pass;
          }
           pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MFe",i),
                          FSmssm.get_physical().MFe(i-1)); 
           if(pass == false){
             OUTPUT << "returning after test fail on MFe getter with index " 
                    << i << std::endl;
             OUTPUT << "string getter gives " 
                    << mssm.mssm_ph.get_Pole_Mass("MFe",i) << std::endl;
             OUTPUT << "struct in model has " 
                    << FSmssm.get_physical().MFe(i-1) << std::endl;
             OUTPUT << TAGerr << TAGfatal << TAGeom;
             return pass;
          }
           pass = is_equal(mssm.mssm_ph.get_Pole_Mass("MFd",i),
                          FSmssm.get_physical().MFd(i-1)); 
          if(pass == false){
             OUTPUT << "returning after test fail on MFd getter with index " 
                    << i << std::endl;
             OUTPUT << "string getter gives " 
                    << mssm.mssm_ph.get_Pole_Mass("MFd",i) << std::endl;
             OUTPUT << "struct in model has " 
                    << FSmssm.get_physical().MFd(i-1) << std::endl;
             OUTPUT << TAGerr << TAGfatal << TAGeom;
             return pass;
          } 
        }
       
       return pass;
    }
    
     
     template <class M>
     bool TestMssmPoleMixingGets2(MSSMSpec<M> mssm, M FSmssm)
     {
        bool pass = false;
        for(int i=1; i<=6; i++){
           for(int j=1; j<=6; j++){
              pass = is_equal(mssm.mssm_ph.get_Pole_Mixing("ZD",i,j),
                              FSmssm.get_physical_slha().ZD(i-1,j-1)); 
         
              if(pass == false){
                 OUTPUT << "returning test fail on ZD getter with indices " 
                        << i << "," << j << std::endl;
                 OUTPUT << "string getter gives " 
                        << mssm.mssm_ph.get_Pole_Mixing("ZD",i,j) << std::endl;
                 OUTPUT << "struct in model has " 
                        << FSmssm.get_physical_slha().ZD(i-1,j-1) << std::endl;
                 OUTPUT << TAGerr << TAGfatal << TAGeom;
                 return pass;
              }
  
           }
        } 
        for(int i=1; i<=6; i++){
           for(int j=1; j<=6; j++){
              pass = is_equal(mssm.mssm_ph.get_Pole_Mixing("ZU",i,j),
                              FSmssm.get_physical_slha().ZU(i-1,j-1)); 
              if(pass == false){
                 OUTPUT << "returning test fail on ZU getter with indices " 
                        << i << "," << j << std::endl;
                 OUTPUT << "string getter gives " 
                        << mssm.mssm_ph.get_Pole_Mixing("ZU",i,j) << std::endl;
                 OUTPUT << "struct in model has " 
                        << FSmssm.get_physical_slha().ZU(i-1,j-1) << std::endl;
                 OUTPUT << TAGerr << TAGfatal << TAGeom;
                 return pass;
              }
           }
        }

        for(int i=1; i<=6; i++){
           for(int j=1; j<=6; j++){
              pass = is_equal(mssm.mssm_ph.get_Pole_Mixing("ZE",i,j),
                              FSmssm.get_physical_slha().ZE(i-1,j-1)); 
              if(pass == false){
                 OUTPUT << "returning test fail on ZE getter with indices " 
                        << i << "," << j << std::endl;
                 OUTPUT << "string getter gives " 
                        << mssm.mssm_ph.get_Pole_Mixing("ZE",i,j) << std::endl;
                 OUTPUT << "struct in model has " 
                        << FSmssm.get_physical_slha().ZE(i-1,j-1) << std::endl;
                 OUTPUT << TAGerr << TAGfatal << TAGeom;
                 return pass;
              }
           }
        }
        
        for(int i=1; i<=3; i++){
           for(int j=1; j<=3; j++){
              pass = is_equal(mssm.mssm_ph.get_Pole_Mixing("ZV",i,j),
                              FSmssm.get_physical_slha().ZV(i-1,j-1)); 
              if(pass == false){
                 OUTPUT << "returning test fail on ZV getter with indices " 
                        << i << "," << j << std::endl;
                 OUTPUT << "string getter gives " 
                        << mssm.mssm_ph.get_Pole_Mixing("ZV",i,j) << std::endl;
                 OUTPUT << "struct in model has " 
                        << FSmssm.get_physical_slha().ZV(i-1,j-1) << std::endl;
                 OUTPUT << TAGerr << TAGfatal << TAGeom;
                 return pass;
              }
           }
        }
     
        for(int i=1; i<=2; i++){
           for(int j=1; j<=2; j++){
              pass = is_equal(mssm.mssm_ph.get_Pole_Mixing("ZH",i,j),
                              FSmssm.get_physical_slha().ZH(i-1,j-1)); 
              if(pass == false){
                 OUTPUT << "returning test fail on ZH getter with indices " 
                        << i << "," << j << std::endl;
                 OUTPUT << "string getter gives " 
                        << mssm.mssm_ph.get_Pole_Mixing("ZH",i,j) << std::endl;
                 OUTPUT << "struct in model has " 
                        << FSmssm.get_physical_slha().ZH(i-1,j-1);
              }     
           }
        }
        return pass;
     }
        
    template <class M> 
    bool TestMssmPoleGets(MSSMSpec<M> mssm, M FSmssm){
       bool pass = false;
       pass = TestMssmPoleGets0(mssm,FSmssm);
       if(pass == false) return pass;
        pass = TestMssmPoleGets1(mssm,FSmssm);
       if(pass == false) return pass;
       pass = TestMssmPoleMixingGets2(mssm,FSmssm);
       if(pass == false) return pass;
       return pass;
    }

    template <class M>
    bool TestMssmParGets(MSSMSpec<M> mssm, M FSmssm){
       bool pass = false; 
       pass = TestMssmParMass2_0(mssm,FSmssm);
       if(pass == false) return pass;
       pass = TestMssmParMass2_2(mssm,FSmssm);
       if(pass == false) return pass;
       pass = TestMssmParMass1_0(mssm,FSmssm);
       if(pass == false) return pass;
       pass = TestMssmParMass1_2(mssm,FSmssm);
       if(pass == false) return pass;
       pass = TestMssmParMass0_0(mssm,FSmssm);
       if(pass == false) return pass;
       pass = TestMssmParMass0_2(mssm,FSmssm);
       if(pass == false) return pass;
    
       return pass;
    
    }

    template <class Model> 
    void setup(Model& mssm)
    {
       Eigen::Matrix<double,3,3> Yu;
       Eigen::Matrix<double,3,3> Yd;
       Eigen::Matrix<double,3,3> Ye;
       double Mu;
       double g1;
       double g2;
       double g3;
       double vd;
       double vu;
       Eigen::Matrix<double,3,3> TYu;
       Eigen::Matrix<double,3,3> TYd;
       Eigen::Matrix<double,3,3> TYe;
       double BMu;
       Eigen::Matrix<double,3,3> mq2;
       Eigen::Matrix<double,3,3> ml2;
       double mHd2;
       double mHu2;
       Eigen::Matrix<double,3,3> md2;
       Eigen::Matrix<double,3,3> mu2;
       Eigen::Matrix<double,3,3> me2;
       double MassB;
       double MassWB;
       double MassG;
    
       // susy parameters
       Yu << 1.26136e-05, 0, 0,
                       0, 0.00667469, 0,
                       0, 0, 0.857849;
    
       Yd << 0.000242026, 0, 0,
                       0, 0.00529911, 0,
                       0, 0, 0.193602;
    
       Ye << 2.84161e-05, 0, 0,
                       0, 0.00587557, 0,
                       0, 0, 0.10199;
    
       Mu = 627.164;
       g1 = 0.468171;
       g2 = 0.642353;
       g3 = 1.06459;
       vd = 25.0944;
       vu = 242.968;
    
       // soft parameters
       TYu << -0.0144387, 0, 0,
                       0, -7.64037, 0,
                       0, 0, -759.305;
    
       TYd << -0.336207, 0, 0,
                      0, -7.36109, 0,
                      0, 0, -250.124;
    
       TYe << -0.00825134, 0, 0,
                        0, -1.70609, 0,
                        0, 0, -29.4466;
    
       BMu = 52140.8;
    
       mq2 << 1.03883e+06, 0, 0,
                        0, 1.03881e+06, 0,
                        0, 0, 879135;
    
       ml2 << 124856, 0, 0,
                   0, 124853, 0,
                   0, 0, 124142;
    
       mHd2 = 92436.9;
       mHu2 = -380337;
    
       md2 << 954454, 0, 0,
                   0, 954439, 0,
                   0, 0, 934727;
    
       mu2 << 963422, 0, 0,
                   0, 963400, 0,
                   0, 0, 656621;
    
       me2 << 49215.8, 0, 0,
                    0, 49210.9, 0,
                    0, 0, 47759.2;
    
       MassB = 210.328;
       MassWB = 389.189;
       MassG = 1114.45;
    
       // set parameters
       mssm.set_scale(Electroweak_constants::MZ);
       mssm.set_Yu(Yu);
       mssm.set_Yd(Yd);
       mssm.set_Ye(Ye);
       mssm.set_Mu(Mu);
       mssm.set_g1(g1);
       mssm.set_g2(g2);
       mssm.set_g3(g3);
       mssm.set_vd(vd);
       mssm.set_vu(vu);
       mssm.set_TYu(TYu);
       mssm.set_TYd(TYd);
       mssm.set_TYe(TYe);
       mssm.set_BMu(BMu);
       mssm.set_mq2(mq2);
       mssm.set_ml2(ml2);
       mssm.set_mHd2(mHd2);
       mssm.set_mHu2(mHu2);
       mssm.set_md2(md2);
       mssm.set_mu2(mu2);
       mssm.set_me2(me2);
       mssm.set_MassB(MassB);
       mssm.set_MassWB(MassWB);
       mssm.set_MassG(MassG);
    }
    
    void spec_print(Spectrum * spec){
    
       OUTPUT << "spec->runningpars.GetScale() =" << spec->runningpars.GetScale() << std::endl;
       OUTPUT << "map mHd2 "  << spec->runningpars.get_mass2_parameter("mHd2") <<std::endl;
       OUTPUT << "map mHu2 "  << spec->runningpars.get_mass2_parameter("mHu2") <<std::endl;
       OUTPUT << "map BMu "  << spec->runningpars.get_mass2_parameter("BMu") <<std::endl;
       OUTPUT << "map mHd2 "  << spec->runningpars.get_mass2_par("mHd2") <<std::endl;
       OUTPUT << "map mHu2 "  << spec->runningpars.get_mass2_par("mHu2") <<std::endl;
       OUTPUT << "map BMu "  << spec->runningpars.get_mass2_par("BMu") <<std::endl;
      
       OUTPUT << "diff mHd2 "  << spec->runningpars.get_mass2_parameter("mHd2") 
                 -  spec->runningpars.get_mass2_par("mHd2") <<std::endl;
       OUTPUT << "diff mHu2 "  << spec->runningpars.get_mass2_parameter("mHu2") 
                 - spec->runningpars.get_mass2_par("mHu2") <<std::endl;
       OUTPUT << "diff BMu "  << spec->runningpars.get_mass2_parameter("BMu") 
                 -  spec->runningpars.get_mass2_par("BMu") <<std::endl;
    
       OUTPUT << "mq2(1,1) =  " <<  spec->runningpars.get_mass2_parameter("mq2",1,1) << std::endl;
       OUTPUT << "fake mq2(1) =  " <<  spec->runningpars.get_mass2_parameter("mq2",1) << std::endl;
    
       double mgluino_drbar =  spec->runningpars.get_tree_MassEigenstate("MGluino");
       OUTPUT << "mgluino_drbar = " <<mgluino_drbar  << std::endl;
       double mgluino = spec->phys.get_Pole_Mass("MGluino");
       OUTPUT << "mgluino = " << mgluino<< std::endl;
    }

    template <class M>
    void mssm_print(MSSMSpec<M> & mssm){
       
       OUTPUT << "mssm.mssm_drbar_pars.GetScale() =" << mssm.mssm_drbar_pars.GetScale() << std::endl;
       OUTPUT << "map mHd2 "  << mssm.mssm_drbar_pars.get_mass2_parameter("mHd2") <<std::endl;
       OUTPUT << "map mHu2 "  << mssm.mssm_drbar_pars.get_mass2_parameter("mHu2") <<std::endl;
       OUTPUT << "map BMu "  << mssm.mssm_drbar_pars.get_mass2_parameter("BMu") <<std::endl;
       OUTPUT << "map mHd2 "  << mssm.mssm_drbar_pars.get_mass2_par("mHd2") <<std::endl;
       OUTPUT << "map mHu2 "  << mssm.mssm_drbar_pars.get_mass2_par("mHu2") <<std::endl;
       OUTPUT << "map BMu "  << mssm.mssm_drbar_pars.get_mass2_par("BMu") <<std::endl;
      
       OUTPUT << "diff mHd2 "  << mssm.mssm_drbar_pars.get_mass2_parameter("mHd2") 
                 -  mssm.mssm_drbar_pars.get_mass2_par("mHd2") <<std::endl;
       OUTPUT << "diff mHu2 "  << mssm.mssm_drbar_pars.get_mass2_parameter("mHu2") 
                 - mssm.mssm_drbar_pars.get_mass2_par("mHu2") <<std::endl;
       OUTPUT << "diff BMu "  << mssm.mssm_drbar_pars.get_mass2_parameter("BMu") 
                 -  mssm.mssm_drbar_pars.get_mass2_par("BMu") <<std::endl;
    
       OUTPUT << "mq2(1,1) =  " <<  mssm.mssm_drbar_pars.get_mass2_parameter("mq2",1,1) << std::endl;
       OUTPUT << "fake mq2(1) =  " <<  mssm.mssm_drbar_pars.get_mass2_parameter("mq2",1) << std::endl;
    
       double mgluino_drbar =  mssm.mssm_drbar_pars.get_tree_MassEigenstate("MGluino");
       OUTPUT << "mgluino_drbar = " <<mgluino_drbar  << std::endl;
       double mgluino = mssm.mssm_ph.get_Pole_Mass("MGluino");
       OUTPUT << "mgluino = " << mgluino<< std::endl;
    
    }
    
     
    void spec_manipulate(Spectrum * spec) {
       OUTPUT << "inside spectrum_manipulate" <<std::endl;
       double lowscale = spec->runningpars.GetScale();
       double highscale = 1e+15;
       OUTPUT << "lowscale  = " << lowscale << std::endl;
       OUTPUT << "highscale = " << highscale << std::endl;
       spec_print(spec);
       OUTPUT << "Testing stability after running..." << std::endl;
       spec->runningpars.RunToScale(highscale);
       OUTPUT << "after run scale to high scale" << std::endl;
       spec_print(spec);
       spec->runningpars.RunToScale(lowscale);
       OUTPUT << "After run scale back to low scale" << spec->runningpars.GetScale() << std::endl;
       spec_print(spec);
      
    }
    
    template <class M>
    void mssm_manipulate(MSSMSpec<M> & mssm) {
       OUTPUT << "inside mssm_manipulate" <<std::endl;
       double lowscale = mssm.mssm_drbar_pars.GetScale();
       //setting to same scale to test
       mssm.mssm_drbar_pars.SetScale(lowscale);
       double highscale = 1e+15;
       OUTPUT << "Mssm start at lowscale = " << lowscale << std::endl;
       mssm_print(mssm);
       mssm.mssm_drbar_pars.RunToScale(highscale);
       OUTPUT << "after run to highscale" << std::endl;
       mssm_print(mssm);
       mssm.mssm_drbar_pars.RunToScale(lowscale);
       OUTPUT << "after run scale back top low scale" <<  std::endl;
       mssm_print(mssm);
    
    }

    void SM_checks(Spectrum* spec) {
      OUTPUT << "In specbit_test_func3: testing retrieval from Spectrum* with capability SM_spectrum..." << endl;
      OUTPUT << "  Scale: " << spec->runningpars.GetScale() << endl;
      OUTPUT << "  Gauge couplings:" << endl;
      OUTPUT << "  g1: " << spec->runningpars.get_dimensionless_parameter("g1") << endl;
      OUTPUT << "  g2: " << spec->runningpars.get_dimensionless_parameter("g2") << endl;
      OUTPUT << "  g3: " << spec->runningpars.get_dimensionless_parameter("g3") << endl;
      OUTPUT << "  Yukawa couplings:" << endl;
      for (int i=0; i<3; i++) { for (int j=0; j<3; j++) {
        OUTPUT << "  Yu("<<i<<","<<j<<"): " << spec->runningpars.get_dimensionless_parameter("Yu", i, j) << endl;
      }}
      for (int i=0; i<3; i++) { for (int j=0; j<3; j++) {
        OUTPUT << "  Yd("<<i<<","<<j<<"): " << spec->runningpars.get_dimensionless_parameter("Yd", i, j) << endl;
      }}
      for (int i=0; i<3; i++) { for (int j=0; j<3; j++) {
        OUTPUT << "  Ye("<<i<<","<<j<<"): " << spec->runningpars.get_dimensionless_parameter("Ye", i, j) << endl;
      }}
    }

  }
}
