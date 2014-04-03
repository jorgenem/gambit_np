//  GAMBIT: Global and Modular BSM Inference Tool
//  *********************************************
///  \file
///
///  Variadic utilty functions.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///
///  \author Gregory Martinez
///          (gregory.david.martinez@gmail.com)
///  \date Feb 2014
///
///  *********************************************

#ifndef VARIADIC_FUNCTIONS_HPP
#define VARIADIC_FUNCTIONS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <type_traits>

#include <yaml-cpp/yaml.h>

namespace Gambit
{       
        //////////////////////
        //div_ints_by_half
        //////////////////////
        
        template <int low, int hi>
        struct div_ints_by_half
        {
                static const int value = (low + hi) >> 1;
        };
        
        /////////////////////
        //is_vector
        /////////////////////
        
        template <typename T>
        struct is_vector
        {
                static const bool value = false;
                typedef T type;
        };
        
        template <typename T>
        struct is_vector<std::vector<T>>
        {
                static const bool value = true;
                typedef T type;
        };
        
        template <typename T>
        struct is_vector<const std::vector<T>>
        {
                static const bool value = true;
                typedef T type;
        };
        
        template <typename T>
        struct is_vector<std::vector<T> &>
        {
                static const bool value = true;
                typedef T type;
        };
        
        template <typename T>
        struct is_vector<const std::vector<T> &>
        {
                static const bool value = true;
                typedef T type;
        };
        
        //////////////////////////////////////
        //Variadic Node functions
        //////////////////////////////////////
        
        inline const YAML::Node getVariadicNode(const YAML::Node &node, std::string key) 
        {
                return node[key];
        }

        template <typename... args>
        inline const YAML::Node getVariadicNode(const YAML::Node &node, const std::string &key, const args&... keys)
        {
                return getVariadicNode(node[key], keys...);
        }
        
        inline const std::string stringifyVariadic(const std::string &str) {return str;}
        
        template<typename... args>
        inline const std::string stringifyVariadic(const std::string &str, const args&... strs) {return str + ", " + stringifyVariadic(strs...);}
        
//         template <typename T>
//         struct getVariadicNumber_internal;
// 
//         template <>
//         struct getVariadicNumber_internal<void()>
//         {
//                 enum{N = 0};
//         };
// 
//         template <typename T, typename... args>
//         struct getVariadicNumber_internal<void (T, args...)>
//         {
//                 enum{N = 1 + getVariadicNumber_internal<void (args...)>::N};
//         };
// 
//         template <typename... args>
//         struct getVariadicNumber
//         {
//                 enum{N = getVariadicNumber_internal<void (args...)>::N};
//         };
        
        /////////////////////////////////////////////////
        //Square Variadic
        /////////////////////////////////////////////////
        
        double squareVariadic(){return 0.0;}
        
        template <typename... args>
        double squareVariadic(const double &val, const args&... params)
        {
                return val*val + squareVariadic(params...);
        };
        
        ///////////////////////////////////////////////
        //Add Variadic Point
        ///////////////////////////////////////////////
        
        template <int i, int n, int N, typename ret>
        struct addVariadicPt_struct
        {
                template <typename T, typename t, typename t1, typename... args>
                inline static ret reducePt(T& f, const t1& val, const t& in, const args&... params)
                {
                        return addVariadicPt_struct<i+1, n, N, ret>::reducePt(f, val, params..., in);
                }
        };
        
        template <int n, int N, typename ret>
        struct addVariadicPt_struct<n, n, N, ret>
        {
                template <typename T, typename t, typename t1, typename... args>
                inline static ret reducePt(T& f, const t1& val, const t& in, const args&... params)
                {
                        return addVariadicPt_struct<n+1, n, N, ret>::reducePt(f, val, in, params..., val);
                }
        };
        
        template <int n, int N, typename ret>
        struct addVariadicPt_struct<N, n, N, ret>
        {
                template <typename T, typename t1, typename... args>
                inline static ret reducePt(T& f, const t1& val, const args&... params)
                {
                        return f (params...);
                }
        };
        
        template <int low, int hi, int N, typename ret>
        struct addVariadicPt_internal
        {
                template <typename T, typename t1, typename... args>
                inline static ret reducePt(T& f, const unsigned int &i, const t1& val, const args&... params)
                {
                        if (i < div_ints_by_half<low, hi>::value)
                                return addVariadicPt_internal<low, div_ints_by_half<low, hi>::value, N, ret>::reducePt(f, i, val, params...);
                        else if (div_ints_by_half<low, hi>::value == i)
                                return addVariadicPt_struct<0, div_ints_by_half<low, hi>::value, N, ret>::reducePt(f, val, params...);
                        else
                                return addVariadicPt_internal<div_ints_by_half<low, hi>::value, hi, N, ret>::reducePt(f, i, val, params...);
                }
        };
        
        template <int low, int N, typename ret>
        struct addVariadicPt_internal<low, low, N, ret>
        {
                template <typename T, typename t1, typename... args>
                inline static ret reducePt(T& f, const unsigned int &i, const t1& val, const args&... params)
                {
                        return addVariadicPt_struct<0, low, N, ret>::reducePt(f, val, params...);
                }
        };
        
        template <typename ret, typename t1, typename... args>
        auto addVariadicPt(const unsigned int &i, const t1& val, ret &f, const args&... params) -> decltype(f(val, params...))
        {
                return addVariadicPt_internal<0, sizeof...(args)+1, sizeof...(args)+1, decltype(f(params...))>::reducePt(f, i, val, params...);
        }
        
        ///////////////////////////////////////////////
        //remove Variadic Point
        ///////////////////////////////////////////////
        
        template <int i, int n, int N, typename ret>
        struct rmVariadicPt_struct
        {
                template <typename T, typename t, typename... args>
                inline static ret reducePt(T& f, const t& in, const args&... params)
                {
                        return rmVariadicPt_struct<i+1, n, N, ret>::reducePt(f, params..., in);
                }
        };
        
        template <int n, int N, typename ret>
        struct rmVariadicPt_struct<n, n, N, ret>
        {
                template <typename T, typename t, typename... args>
                inline static ret reducePt(T& f, const t& in, const args&... params)
                {
                        return rmVariadicPt_struct<n+1, n, N, ret>::reducePt(f, params...);
                }
        };
        
        template <int n, int N, typename ret>
        struct rmVariadicPt_struct<N, n, N, ret>
        {
                template <typename T, typename... args>
                inline static ret reducePt(T& f, const args&... params)
                {
                        return f (params...);
                }
        };
        
        template <int N, typename ret>
        struct rmVariadicPt_struct<N, N, N, ret>
        {
                template <typename T, typename t, typename... args>
                inline static ret reducePt(T& f, const t& in, const args&... params)
                {
                        return f (params...);
                }
        };
        
        template <int low, int hi, int N, typename ret>
        struct rmVariadicPt_internal
        {
                template <typename T, typename... args>
                inline static ret reducePt(T& f, const unsigned int &i, const args&... params)
                {
                        if (i < div_ints_by_half<low, hi>::value)
                                return rmVariadicPt_internal<low, div_ints_by_half<low, hi>::value, N, ret>::reducePt(f, i, params...);
                        else if (div_ints_by_half<low, hi>::value == i)
                                return rmVariadicPt_struct<0, div_ints_by_half<low, hi>::value, N, ret>::reducePt(f, params...);
                        else
                                return rmVariadicPt_internal<div_ints_by_half<low, hi>::value, hi, N, ret>::reducePt(f, i, params...);
                }
        };
        
        template <int low, int N, typename ret>
        struct rmVariadicPt_internal<low, low, N, ret>
        {
                template <typename T, typename... args>
                inline static ret reducePt(T& f, const unsigned int &i, const args&... params)
                {
                        return rmVariadicPt_struct<0, low, N, ret>::reducePt(f, params...);
                }
        };
        
        template <typename ret, typename T, typename... args>
        auto rmVariadicPt(const unsigned int &i, ret &f, const T& in, const args&... params) -> decltype(f(params...))
        {
                return rmVariadicPt_internal<0, sizeof...(args)+1, sizeof...(args)+1, decltype(f(params...))>::reducePt(f, i, in, params...);
        }
        
        ///////////////////////////////////////////////
        //input Variadic Point
        ///////////////////////////////////////////////
        
        template <int i, int n, int N, typename ret>
        struct inputVariadicPt_struct
        {
                template <typename T, typename t, typename t1, typename... args>
                inline static ret reducePt(T& f, const t1& val, const t& in, const args&... params)
                {
                        return inputVariadicPt_struct<i+1, n, N, ret>::reducePt(f, val, params..., in);
                }
        };
        
        template <int n, int N, typename ret>
        struct inputVariadicPt_struct<n, n, N, ret>
        {
                template <typename T, typename t, typename t1, typename... args>
                inline static ret reducePt(T& f, const t1& val, const t& in, const args&... params)
                {
                        return inputVariadicPt_struct<n+1, n, N, ret>::reducePt(f, val, params..., val);
                }
        };
        
        template <int n, int N, typename ret>
        struct inputVariadicPt_struct<N, n, N, ret>
        {
                template <typename T, typename t1, typename... args>
                inline static ret reducePt(T& f, const t1& val, const args&... params)
                {
                        return f (params...);
                }
        };
        
        template <int low, int hi, int N, typename ret>
        struct inputVariadicPt_internal
        {
                template <typename T, typename t1, typename... args>
                inline static ret reducePt(T& f, const unsigned int &i, const t1& val, const args&... params)
                {
                        if (i < div_ints_by_half<low, hi>::value)
                                return inputVariadicPt_internal<low, div_ints_by_half<low, hi>::value, N, ret>::reducePt(f, i, val, params...);
                        else if (div_ints_by_half<low, hi>::value == i)
                                return inputVariadicPt_struct<0, div_ints_by_half<low, hi>::value, N, ret>::reducePt(f, val, params...);
                        else
                                return inputVariadicPt_internal<div_ints_by_half<low, hi>::value, hi, N, ret>::reducePt(f, i, val, params...);
                }
        };
        
        template <int low, int N, typename ret>
        struct inputVariadicPt_internal<low, low, N, ret>
        {
                template <typename T, typename t1, typename... args>
                inline static ret reducePt(T& f, const unsigned int &i, const t1& val, const args&... params)
                {
                        return inputVariadicPt_struct<0, low, N, ret>::reducePt(f, val, params...);
                }
        };
        
        template <typename ret, typename t1, typename... args>
        auto inputVariadicPt(const unsigned int &i, const t1& val, ret &f, const args&... params) -> decltype(f(params...))
        {
                return inputVariadicPt_internal<0, sizeof...(args), sizeof...(args), decltype(f(params...))>::reducePt(f, i, val, params...);
        }
        
        //////////////////////////////////////////////
        //input variadic function
        //////////////////////////////////////////////
        
        template<typename T, typename... args>
        inline double inputVariadicFunction(T&f, std::vector<double>::const_iterator begin, std::vector<double>::const_iterator end,
                double in1, double in2, double in3, double in4, double in5, double in6, double in7, double in8, double in9, double in10)
        {
                std::cout << "inputVariadicFunction:  Max argument length exceeded." << std::endl;
                return 0.0;
        }
        
        template<typename T, typename... args>
        inline double inputVariadicFunction(T&f, std::vector<double>::const_iterator begin, std::vector<double>::const_iterator end, const args&... params)
        {
                if(begin != end)
                {
                        return inputVariadicFunction(f, begin+1, end, params..., *begin);
                }
                else
                {
                        return f(params...);
                }
        }
        
        ///////////////////////////////////////////////
        //Input and Output Variadic functions
        ///////////////////////////////////////////////
        
        template <typename T>
        inline typename std::enable_if<is_vector<T>::value, int>::type
        getVariadicVectorSize(const T &in){return in.size();}
        
        template <typename T>
        inline typename std::enable_if<!is_vector<T>::value, int>::type
        getVariadicVectorSize(const T &in){return 0;}
        
        inline int getVariadicMaxVector(){return 0;}
        
        template <typename T, typename... args>
        inline int getVariadicMaxVector (const T &in, const args&... params)
        {
                int N0 = getVariadicMaxVector(params...);
                int N = getVariadicVectorSize(in);

                return (N > N0 && N0 != 0) ? N0 : N;
        };
        
        inline void inputVariadicVector(std::vector<double>::iterator vec){}
        
        template <typename... args>
        inline void inputVariadicVector(std::vector<double>::iterator vec, const double &val, const args&... params)
        {
                *vec = val; 
                inputVariadicVector(vec+1, params...);
        }
        
        inline void outputVariadicVector(std::vector<double>::const_iterator vec) {}
        
        template <typename... args>
        inline void outputVariadicVector(std::vector<double>::const_iterator vec, double &val, args&... params)
        {
                val = *vec;
                outputVariadicVector(vec+1, params...);
        }
        
        template <typename ret>
        inline ret getVariadicPt(const unsigned int& i)
        {
                return 0.0;
        }
        
        template <typename t, typename ret = t, typename... args>
        inline ret getVariadicPt(const unsigned int& i, const t& in, const args&... params)
        {
                return (i) ? getVariadicPt<ret>(i-1, params...) : in;
        }
        
        ///////////////////////////////
        //mult_types
        ///////////////////////////////
        
        template <typename... args>
        struct mult_types
        {
                typedef void type (args...);
        };
        
        ////////////////////////////
        //is_same_type
        ////////////////////////////
        
        template <typename T, typename type>
        struct is_same_type_internal;
        
        template <typename type>
        struct is_same_type_internal <void (), type>
        {
               static const bool value = false;
        };
        
        template <typename T, typename... args>
        struct is_same_type_internal <void (T, args...), T>
        {
                static const bool value = true;
        };
        
        template <typename type, typename T, typename... args>
        struct is_same_type_internal <void (T, args...), type>
        {
                static const bool value = is_same_type_internal <void (args...), type>::value;
        };
        
        template <typename type, typename T>
        struct is_same_type
        {
                static const bool value = false;
        };
        
        template <typename T>
        struct is_same_type <T, T>
        {
                static const bool value = true;
        };
        
        template <typename T, typename... args>
        struct is_same_type <mult_types<args...>, T>
        {
                static const bool value = is_same_type_internal <typename mult_types<args...>::type, T>::value;
        };
        
        ///////////////////////////
        //is_one_member
        ///////////////////////////
        
        template <typename type, typename T>
        struct is_one_member_internal;
        
        template <typename type>
        struct is_one_member_internal <type, void ()>
        {
                static const bool value = false;
        };
        
        template <typename type, typename T, typename... args>
        struct is_one_member_internal <type, void (T, args...)>
        {
                static const bool value = is_same_type<type, T>::value || is_one_member_internal<type, void (args...)>::value;
        };
        
        template <typename type, typename... args>
        struct is_one_member
        {
                static const bool value = is_one_member_internal<type, void (args...)>::value;
        };
        
        //////////////////////////////
        //is_all_member
        //////////////////////////////
        
        template <typename type, typename T>
        struct is_all_member_internal;
        
        template <typename type>
        struct is_all_member_internal <type, void ()>
        {
                static const bool value = true;
        };
        
        template <typename type, typename T, typename... args>
        struct is_all_member_internal <type, void (T, args...)>
        {
                static const bool value = is_same_type<type, T>::value && is_all_member_internal<type, void (args...)>::value;
        };
        
        template <typename type, typename... args>
        struct is_all_member
        {
                static const bool value = is_all_member_internal<type, void (args...)>::value;
        };
        
        ///////////////////////////////////
        //is_one_member_vector
        ///////////////////////////////////
        
        template <typename T>
        struct is_one_member_vector_internal;
        
        template <>
        struct is_one_member_vector_internal < void ()>
        {
                static const bool value = false;
        };
        
        template <typename T, typename... args>
        struct is_one_member_vector_internal <void (std::vector<T>, args...)>
        {
                static const bool value = true;
        };
        
        template <typename T, typename... args>
        struct is_one_member_vector_internal <void (std::vector<T> &, args...)>
        {
                static const bool value = true;
        };
        
        template <typename T, typename... args>
        struct is_one_member_vector_internal <void (const std::vector<T> &, args...)>
        {
                static const bool value = true;
        };
        
        template <typename T, typename... args>
        struct is_one_member_vector_internal <void (T, args...)>
        {
                static const bool value = is_one_member_vector_internal<void (args...)>::value;
        };
        
        template <typename... args>
        struct is_one_member_vector
        {
                static const bool value = is_one_member_vector_internal<void (args...)>::value;
        };
                
        //////////////////////////////
        //is_all_member_vector
        //////////////////////////////
        
        template <typename T>
        struct is_all_member_vector_internal;
        
        template <>
        struct is_all_member_vector_internal <void ()>
        {
                static const bool value = true;
        };
        
        template <typename T, typename... args>
        struct is_all_member_vector_internal <void (std::vector<T>, args...)>
        {
                static const bool value = is_all_member_vector_internal<void (args...)>::value;
        };
        
        template <typename T, typename... args>
        struct is_all_member_vector_internal <void (std::vector<T> &, args...)>
        {
                static const bool value = is_all_member_vector_internal<void (args...)>::value;
        };
        
        template <typename T, typename... args>
        struct is_all_member_vector_internal <void (const std::vector<T> &, args...)>
        {
                static const bool value = is_all_member_vector_internal<void (args...)>::value;
        };
        
        template <typename T, typename... args>
        struct is_all_member_vector_internal <void (T, args...)>
        {
                static const bool value = false;
        };
        
        template <typename... args>
        struct is_all_member_vector
        {
                static const bool value = is_all_member_vector_internal<void (args...)>::value;
        };
        
        /////////////////////////////
        //enable_if's
        /////////////////////////////
        
        template <typename T, typename ret, typename... args>
        struct enable_if_one_member
        {
                typedef std::enable_if<is_one_member<T, args...>::value, ret> type;
        };
        
        template <typename T, typename ret, typename... args>
        struct enable_if_all_member
        {
                typedef std::enable_if<is_all_member<T, args...>::value, ret> type;
        };
        
        template <typename ret, typename... args>
        struct enable_if_one_member_vector
        {
                typedef std::enable_if<is_one_member_vector<args...>::value, ret> type;
        };
        
        template <typename ret, typename... args>
        struct enable_if_all_member_vector
        {
                typedef std::enable_if<is_all_member_vector<args...>::value, ret> type;
        };
        
        //////////////////////////////////////
        //enable_if_not's
        //////////////////////////////////////
        
        template <typename T, typename ret, typename... args>
        struct enable_if_not_one_member
        {
                typedef std::enable_if<!is_one_member<T, args...>::value, ret> type;
        };
        
        template <typename T, typename ret, typename... args>
        struct enable_if_not_all_member
        {
                typedef std::enable_if<!is_all_member<T, args...>::value, ret> type;
        };
        
        template <typename ret, typename... args>
        struct enable_if_not_one_member_vector
        {      
                typedef std::enable_if<!is_one_member_vector<args...>::value, ret> type;
        };
        
        template <typename ret, typename... args>
        struct enable_if_not_all_member_vector
        {
                typedef std::enable_if<!is_all_member_vector<args...>::value, ret> type;
        };
}

#endif
