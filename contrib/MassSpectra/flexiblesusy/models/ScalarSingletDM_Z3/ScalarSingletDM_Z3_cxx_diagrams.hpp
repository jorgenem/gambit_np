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

// File generated at Sat 26 May 2018 14:35:50

/**
 * @file ScalarSingletDM_Z3_cxx_diagrams.hpp
 *
 * This file was generated at Sat 26 May 2018 14:35:50 with FlexibleSUSY
 * 2.0.1 and SARAH 4.12.2 .
 */

#ifndef ScalarSingletDM_Z3_CXXDIAGRAMS_H
#define ScalarSingletDM_Z3_CXXDIAGRAMS_H

#include "numerics2.hpp"
#include "wrappers.hpp"

#include <array>
#include <boost/range/join.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/adapted/boost_array.hpp>
#include <boost/array.hpp>

#include <boost/version.hpp>

#if BOOST_VERSION >= 105800
#include <boost/fusion/include/move.hpp>
#else
#include <boost/fusion/include/copy.hpp>
#endif

#define INPUTPARAMETER(p) context.model.get_input().p
#define MODELPARAMETER(p) context.model.get_##p()
#define DERIVEDPARAMETER(p) context.model.p()
#define PHASE(p) context.model.get_##p()

namespace flexiblesusy {
namespace cxx_diagrams {
namespace impl {
/** \brief Helper template for make_array
 */
template<class ForwardIterator, std::size_t N, class T>
struct make_array_it {
   using decayed_type = typename std::decay<
                        typename std::iterator_traits<ForwardIterator>::value_type
                        >::type;
   static constexpr bool use_cast = !std::is_same<decayed_type, T>::value;

   template<class... Args>
   static auto iterate(ForwardIterator begin, Args &&...args)
   -> typename std::enable_if<
   !use_cast,
   std::array<T, N + sizeof...(Args)>
   >::type {
      ForwardIterator copy(begin);
      return make_array_it<ForwardIterator, N-1, T>::iterate(++begin,
            std::forward<Args>(args)..., *copy);
   }

   template<class... Args>
   static auto iterate(ForwardIterator begin, Args &&...args)
   -> typename std::enable_if<
   use_cast,
   std::array<T, N + sizeof...(Args)>
   >::type {
      ForwardIterator copy(begin);
      return make_array_it<ForwardIterator, N-1, T>::iterate(++begin,
            std::forward<Args>(args)..., T(*copy));
   }
};

/** \brief Specialized helper template for make_array */
template<class ForwardIterator, class T>
struct make_array_it<ForwardIterator, 0, T> {
   template<class... Args>
   static auto iterate(ForwardIterator /* begin */, Args &&...args)
   -> std::array<T, sizeof...(Args)> {
      return std::array<T, sizeof...(Args)>{ std::forward<Args>(args)... };
   }
};

/** \class make_array
 * \brief Facility for easy construction of \a std::array
 *        objects.
 * \tparam N The size of the to be constructed array
 * \tparam T The type of the objects the array should hold
 *           or void (by default) if the type should be
 *           inferred.
 */
template<std::size_t N, class T = void>
struct make_array {
   /** \brief Construct a \a std::array by copying values
    *         from a range.
    * \tparam ForwardIterator The iterator type (usually inferred)
    * \param[in] begin The iterator marking the beginning of
    *                  the range to be copied.
    * \returns A \a std::array holding the desired objects.
    * \warning \a begin must be incrementable sufficiently
    *          often (\a N times) otherwise the behaviour
    *          is undefined.
    */
   template<class ForwardIterator>
   static auto iterate(ForwardIterator begin)
   -> std::array<typename std::conditional<
   std::is_void<T>::value,
       typename std::iterator_traits<ForwardIterator>::value_type,
       T
   >::type, N> {
      using value_type = typename std::conditional<
      std::is_void<T>::value,
      typename std::iterator_traits<ForwardIterator>::value_type,
      T
      >::type;

      return make_array_it<ForwardIterator, N, value_type>::iterate(begin);
   }
};

template<class Array1, class Array2>
auto concatenate(const Array1& a1, const Array2& a2)
-> std::array<
typename std::common_type<
typename Array1::value_type,
         typename Array1::value_type
         >::type,
         std::tuple_size<Array1>::value + std::tuple_size<Array2>::value>
         {
            using value_type = typename std::common_type<
                               typename Array1::value_type,
            typename Array1::value_type
            >::type;
            constexpr auto size = std::tuple_size<Array1>::value + std::tuple_size<Array2>::value;

            auto range1 = boost::make_iterator_range(boost::begin(a1),
                          boost::end(a1));
            auto range2 = boost::make_iterator_range(boost::begin(a2),
                          boost::end(a2));
            auto joined = boost::join(range1, range2);

            return make_array<size, value_type>::iterate(boost::begin(joined));
         }

         /**
          * @class IndexBounds<N>
          * @brief A class representing multiple (N) index ranges.
          *
          * N is a non-negative integer.
          * The ranges are specified the c++ way; The range begins are inclusive
          * and the range ends are exclusive. Misspecified ranges result in
          * undefined behaviour!
          * The intended use is to iterate over an IndexBounds<N> using the normal
          * begin()/end() syntax, which will iterate over every possible combination
          * of the indices within their respective ranges.
          * The ranges are const! Once they are initialized they cannot be changed.
          * Initialization is done like = {{ beg1, beg2, ... }, { end1, end2, ... }}
          */
         template<int N> struct IndexBounds;

/**
 * @class IndexIterator<N>
 * @brief The iterator class used by IndexBounds<N>.
 *
 * It only fulfils the input iterator requirements!
 * The value_type is a const std::array<int, N>
 * containing the current indices.
 * Incrementing the iterator results in a new index combination.
 */
template<int N> struct IndexIterator {
   using difference_type = typename std::array<int, N>::difference_type;
   using value_type = int;
   using pointer = int*;
   using reference = int&;
   using iterator_category = std::input_iterator_tag;

   friend class IndexBounds<N>;
private:
   const IndexBounds<N>& bounds;
   std::array<int, N> indices;

   IndexIterator(const IndexBounds<N>& b, std::array<int, N>&& i)
      : bounds(b), indices(std::move(i)) {}
public:
   const std::array<int, N>& operator*() const
   {
      return indices;
   }

   const std::array<int, N>* operator->() const
   {
      return &indices;
   }

   IndexIterator& operator++()
   {
      for (int i = 0; i != N; i++) {
         indices[i]++;
         if (indices[i] == bounds.indexEnd[i]) {
            indices[i] = bounds.indexBegin[i];
            continue;
         }

         return *this;
      }

      indices = make_array<N>::iterate(bounds.indexEnd);
      return *this;
   }

   IndexIterator operator++(int)
   {
      IndexIterator it(*this);
      this->operator++();
      return it;
   }

   template<int M>
   friend bool operator==(const IndexIterator<M>& it1, const IndexIterator<M>& it2);
};

template<int N>
bool operator==(const IndexIterator<N>& it1, const IndexIterator<N>& it2)
{
   return it1.indices == it2.indices;
}

template<int N>
bool operator!=(const IndexIterator<N>& it1, const IndexIterator<N>& it2)
{
   return !(it1 == it2);
}

template<int N> struct IndexBounds {
   typedef const std::array<int, N> indices_type;

   int indexBegin[N];
   int indexEnd[N];

   typedef IndexIterator<N> const_iterator;

   const_iterator begin() const
   {
      return const_iterator(*this, make_array<N>::iterate(indexBegin));
   }

   const_iterator end() const
   {
      return const_iterator(*this, make_array<N>::iterate(indexEnd));
   }
};

template<> struct IndexBounds<0> {
   using indices_type = const std::array<int, 0>;
   using const_iterator = indices_type*;

   const indices_type dummyIndex = {};

   const_iterator begin() const
   {
      return &dummyIndex;
   }
   const_iterator end() const
   {
      return (begin()+1);
   }
};

} // namespace impl

// Lorentz conjugate fermions
template<class Field> struct bar {
   static constexpr int numberOfGenerations = Field::numberOfGenerations;
   static constexpr int numberOfFieldIndices = Field::numberOfFieldIndices;
   using smFlags = typename Field::smFlags;

   static constexpr double electric_charge = - Field::electric_charge;

   using type = bar<Field>;
   using lorentz_conjugate = Field;
};

// Lorentz conjugate bosons
template<class Field> struct conj {
   static constexpr int numberOfGenerations = Field::numberOfGenerations;
   static constexpr int numberOfFieldIndices = Field::numberOfFieldIndices;
   using smFlags = typename Field::smFlags;

   static constexpr double electric_charge = - Field::electric_charge;

   using type = conj<Field>;
   using lorentz_conjugate = Field;
};

// Double Lorentz conjugation
template<class Field> struct bar<bar<Field>> {
   using type = Field;
};
template<class Field> struct conj<conj<Field>> {
   using type = Field;
};

// Remove Lorentz conjugation
template<class Field> struct remove_lorentz_conjugation {
   using type = Field;
};

template<class Field> struct remove_lorentz_conjugation<bar<Field>> {
   using type = Field;
};

template<class Field> struct remove_lorentz_conjugation<conj<Field>> {
   using type = Field;
};


// Declare a type that can hold the field indices for any given field
template<class Field> struct field_indices {
   using type = std::array<int, Field::numberOfFieldIndices>;
};

template<class Field>
typename std::enable_if<Field::numberOfGenerations != 1, bool>::type
isSMField(const typename field_indices<Field>::type& indices)
{
   boost::array<bool,Field::numberOfGenerations> smFlags;

#if BOOST_VERSION >= 105800
   boost::fusion::move(typename Field::smFlags(), smFlags);
#else
   boost::fusion::copy(typename Field::smFlags(), smFlags);
#endif

   return smFlags[indices[0]];
}

template<class Field>
typename std::enable_if<Field::numberOfGenerations == 1, bool>::type
isSMField(const typename field_indices<Field>::type& /* indices */)
{
   return boost::mpl::at_c<typename Field::smFlags,0>::type::value;
}

struct VG {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, true>;
   static constexpr int numberOfFieldIndices = 1;
   static constexpr double electric_charge = 0;
   using lorentz_conjugate = VG;
};

struct gG {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, true>;
   static constexpr int numberOfFieldIndices = 1;
   static constexpr double electric_charge = 0;
   using lorentz_conjugate = bar<gG>::type;
};

struct Hp {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, true>;
   static constexpr int numberOfFieldIndices = 0;
   static constexpr double electric_charge = -1;
   using lorentz_conjugate = conj<Hp>::type;
};

struct ss {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, false>;
   static constexpr int numberOfFieldIndices = 0;
   static constexpr double electric_charge = 0;
   using lorentz_conjugate = conj<ss>::type;
};

struct Fv {
   static constexpr int numberOfGenerations = 3;
   using smFlags = boost::mpl::vector_c<bool, true, true, true>;
   static constexpr int numberOfFieldIndices = 1;
   static constexpr double electric_charge = 0;
   using lorentz_conjugate = bar<Fv>::type;
};

struct Ah {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, true>;
   static constexpr int numberOfFieldIndices = 0;
   static constexpr double electric_charge = 0;
   using lorentz_conjugate = Ah;
};

struct hh {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, true>;
   static constexpr int numberOfFieldIndices = 0;
   static constexpr double electric_charge = 0;
   using lorentz_conjugate = hh;
};

struct VP {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, true>;
   static constexpr int numberOfFieldIndices = 0;
   static constexpr double electric_charge = 0;
   using lorentz_conjugate = VP;
};

struct VZ {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, true>;
   static constexpr int numberOfFieldIndices = 0;
   static constexpr double electric_charge = 0;
   using lorentz_conjugate = VZ;
};

struct gP {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, true>;
   static constexpr int numberOfFieldIndices = 0;
   static constexpr double electric_charge = 0;
   using lorentz_conjugate = bar<gP>::type;
};

struct gZ {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, true>;
   static constexpr int numberOfFieldIndices = 0;
   static constexpr double electric_charge = 0;
   using lorentz_conjugate = bar<gZ>::type;
};

struct gWp {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, true>;
   static constexpr int numberOfFieldIndices = 0;
   static constexpr double electric_charge = 1;
   using lorentz_conjugate = bar<gWp>::type;
};

struct gWpC {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, true>;
   static constexpr int numberOfFieldIndices = 0;
   static constexpr double electric_charge = -1;
   using lorentz_conjugate = bar<gWpC>::type;
};

struct Fd {
   static constexpr int numberOfGenerations = 3;
   using smFlags = boost::mpl::vector_c<bool, true, true, true>;
   static constexpr int numberOfFieldIndices = 2;
   static constexpr double electric_charge = -0.3333333333333333;
   using lorentz_conjugate = bar<Fd>::type;
};

struct Fu {
   static constexpr int numberOfGenerations = 3;
   using smFlags = boost::mpl::vector_c<bool, true, true, true>;
   static constexpr int numberOfFieldIndices = 2;
   static constexpr double electric_charge = 0.6666666666666666;
   using lorentz_conjugate = bar<Fu>::type;
};

struct Fe {
   static constexpr int numberOfGenerations = 3;
   using smFlags = boost::mpl::vector_c<bool, true, true, true>;
   static constexpr int numberOfFieldIndices = 1;
   static constexpr double electric_charge = -1;
   using lorentz_conjugate = bar<Fe>::type;
};

struct VWp {
   static constexpr int numberOfGenerations = 1;
   using smFlags = boost::mpl::vector_c<bool, true>;
   static constexpr int numberOfFieldIndices = 0;
   static constexpr double electric_charge = 1;
   using lorentz_conjugate = conj<VWp>::type;
};

// Named fields
using Photon = VP;
using Electron = Fe;

// Fields that are their own Lorentz conjugates.
template<> struct conj<VG> { using type = VG; };
template<> struct conj<Ah> { using type = Ah; };
template<> struct conj<hh> { using type = hh; };
template<> struct conj<VP> { using type = VP; };
template<> struct conj<VZ> { using type = VZ; };



/**
* @class SingleComponentedVertex
* @brief A vertex whose value can be represented by a single complex number
*/
class SingleComponentedVertex {
private:
   std::complex<double> val; ///< The value
public:
   SingleComponentedVertex(std::complex<double> v)
      : val(v) {}

   /**
   * @fn value
   * @brief Returns the value of the vertex.
   */
   std::complex<double> value() const
   {
      return val;
   }

   /**
   * @fn isZero
   * @brief Tests whether the value is numerically significant
   */
   bool isZero() const
   {
      return (is_zero(val.real()) && is_zero(val.imag()));
   }
};

/**
* @class LeftAndRightComponentedVertex
* @brief A vertex whose value can be represented by two complex numbers
*/
class LeftAndRightComponentedVertex {
private:
   std::pair<std::complex<double>, std::complex<double>> value;  ///< The values
public:
   LeftAndRightComponentedVertex(const std::complex<double>& left,
                                 const std::complex<double>& right)
      : value(left, right) {}

   /**
   * @fn left
   * @brief Returns the left component of the vertex.
   */
   std::complex<double> left() const
   {
      return value.first;
   }

   /**
   * @fn right
   * @brief Returns the left component of the vertex.
   */
   std::complex<double> right() const
   {
      return value.second;
   }

   /**
   * @fn isZero
   * @brief Tests whether the values are numerically significant
   */
   bool isZero() const
   {
      return (is_zero(value.first.real()) && is_zero(value.first.imag()) &&
              is_zero(value.second.real()) && is_zero(value.second.imag()));
   }
};

/**
 * @class VertexData<F...>
 * @brief VertexData data for a vertex with the fields specified by F....
 */
template<class ...Fields> struct VertexData;

struct EvaluationContext;

/**
 * @class Vertex<F...>
 * @brief A template that represents a vertex with open field indices.
 *
 * All elements in F... have to be publicly derived from Field.
 * To obtain a conrete value, use the static member function evaluate()
 * along with the desired field indices.
 */
template<class ...F> class Vertex {
   using Data = VertexData<F...>;
   using bounds_type = typename std::decay<decltype(Data::index_bounds)>::type;
public:
   using vertex_type = typename Data::vertex_type;
   using indices_type = typename decltype(Data::index_bounds)::indices_type;

   static constexpr bounds_type index_bounds = Data::index_bounds;

   /**
    * @fn fieldIndices
    * @brief Returns the subset of indices belonging to
    *        a field at a given index.
    * @param indices The complete set of indices for all fields
    *
    * The field indexing is in the same order as the template arguments
    * and starts at 0.
    */
   template<int fieldIndex>
   static
   std::array<int, Data::fieldIndexStart[fieldIndex+1] - Data::fieldIndexStart[fieldIndex]>
   fieldIndices(const indices_type& indices)
   {
      constexpr auto length = Data::fieldIndexStart[fieldIndex+1] - Data::fieldIndexStart[fieldIndex];
      constexpr auto offset = Data::fieldIndexStart[fieldIndex];
      auto begin = indices.begin() + offset;
      return impl::make_array<length>::iterate(begin);
   }

   /**
    * @fn vertex
    * @brief Calculates the vertex for a given set of field indices.
    * @param indices The field indices
    * @param context The evaluation context
    */
   static vertex_type evaluate(const indices_type& indices, const EvaluationContext& context);
};


/**
* @class EvaluationContext
* @brief Represents an evaluation context.
*
* It simply contains a reference to a model object.
* All computational functions are forwarded to that object,
* e.g. mass calculation functions.
*/
struct EvaluationContext {
   ScalarSingletDM_Z3_mass_eigenstates& model; ///< The model object.

   template<class Field>
   double mass(const typename field_indices<Field>::type& indices) const
   {
      using CleanField = typename remove_lorentz_conjugation<Field>::type;
      return mass_impl<CleanField>(indices);
   }

private:
   template<class Field>
   double mass_impl(const typename field_indices<Field>::type& indices) const;
};


template<> struct VertexData<bar<Fe>::type, Ah, Fe>
{
   static constexpr impl::IndexBounds<2> index_bounds = { { 0, 0 }, { 3, 3 } };
   static constexpr int fieldIndexStart[4] = { 0, 1, 1, 2 };
   using vertex_type = LeftAndRightComponentedVertex;
};

template<> struct VertexData<Fe, Ah, bar<Fe>::type>
{
   static constexpr impl::IndexBounds<2> index_bounds = { { 0, 0 }, { 3, 3 } };
   static constexpr int fieldIndexStart[4] = { 0, 1, 1, 2 };
   using vertex_type = LeftAndRightComponentedVertex;
};

template<> struct VertexData<VP, bar<Fe>::type, Fe>
{
   static constexpr impl::IndexBounds<2> index_bounds = { { 0, 0 }, { 3, 3 } };
   static constexpr int fieldIndexStart[4] = { 0, 0, 1, 2 };
   using vertex_type = LeftAndRightComponentedVertex;
};

template<> struct VertexData<bar<Fe>::type, Fv, conj<Hp>::type>
{
   static constexpr impl::IndexBounds<2> index_bounds = { { 0, 0 }, { 3, 3 } };
   static constexpr int fieldIndexStart[4] = { 0, 1, 2, 2 };
   using vertex_type = LeftAndRightComponentedVertex;
};

template<> struct VertexData<Fe, bar<Fv>::type, Hp>
{
   static constexpr impl::IndexBounds<2> index_bounds = { { 0, 0 }, { 3, 3 } };
   static constexpr int fieldIndexStart[4] = { 0, 1, 2, 2 };
   using vertex_type = LeftAndRightComponentedVertex;
};

template<> struct VertexData<VP, Hp, conj<Hp>::type>
{
   static constexpr impl::IndexBounds<0> index_bounds{};
   static constexpr int fieldIndexStart[4] = { 0, 0, 0, 0 };
   using vertex_type = SingleComponentedVertex;
};

template<> struct VertexData<bar<Fe>::type, hh, Fe>
{
   static constexpr impl::IndexBounds<2> index_bounds = { { 0, 0 }, { 3, 3 } };
   static constexpr int fieldIndexStart[4] = { 0, 1, 1, 2 };
   using vertex_type = LeftAndRightComponentedVertex;
};

template<> struct VertexData<Fe, hh, bar<Fe>::type>
{
   static constexpr impl::IndexBounds<2> index_bounds = { { 0, 0 }, { 3, 3 } };
   static constexpr int fieldIndexStart[4] = { 0, 1, 1, 2 };
   using vertex_type = LeftAndRightComponentedVertex;
};


template<> inline
Vertex<bar<Fe>::type, Ah, Fe>::vertex_type
Vertex<bar<Fe>::type, Ah, Fe>::evaluate(const indices_type& indices, const EvaluationContext& context)
{
   const int gt1 = indices[0];
   const int gt3 = indices[1];
   const auto Ye = MODELPARAMETER(Ye);
   const auto Ve = MODELPARAMETER(Ve);
   const auto Ue = MODELPARAMETER(Ue);

   const std::complex<double> left = std::complex<double>(0.,0.7071067811865475)*SUM(j2,0,2,Conj(Ve(gt3,j2))*SUM(j1,0,2,Conj(Ue(gt1,j1))*Ye(j1,j2)));

   const std::complex<double> right = std::complex<double>(0.,-0.7071067811865475)*SUM(j2,0,2,SUM(j1,0,2,Conj(Ye(j1,j2))*Ue(gt3,j1))*Ve(gt1,j2));

   return vertex_type(left, right);
}

template<> inline
Vertex<Fe, Ah, bar<Fe>::type>::vertex_type
Vertex<Fe, Ah, bar<Fe>::type>::evaluate(const indices_type& indices, const EvaluationContext& context)
{
   const int gt1 = indices[0];
   const int gt3 = indices[1];
   const auto Ye = MODELPARAMETER(Ye);
   const auto Ve = MODELPARAMETER(Ve);
   const auto Ue = MODELPARAMETER(Ue);

   const std::complex<double> left = std::complex<double>(0.,0.7071067811865475)*SUM(j2,0,2,Conj(Ve(gt1,j2))*SUM(j1,0,2,Conj(Ue(gt3,j1))*Ye(j1,j2)));

   const std::complex<double> right = std::complex<double>(0.,-0.7071067811865475)*SUM(j2,0,2,SUM(j1,0,2,Conj(Ye(j1,j2))*Ue(gt1,j1))*Ve(gt3,j2));

   return vertex_type(left, right);
}

template<> inline
Vertex<VP, bar<Fe>::type, Fe>::vertex_type
Vertex<VP, bar<Fe>::type, Fe>::evaluate(const indices_type& indices, const EvaluationContext& context)
{
   const int gt2 = indices[0];
   const int gt3 = indices[1];
   const auto g1 = MODELPARAMETER(g1);
   const auto g2 = MODELPARAMETER(g2);
   const auto ThetaW = DERIVEDPARAMETER(ThetaW);

   const std::complex<double> left = 0.5*KroneckerDelta(gt2,gt3)*(0.7745966692414834*g1*Cos(ThetaW) + g2*Sin(ThetaW));

   const std::complex<double> right = 0.7745966692414834*g1*Cos(ThetaW)*KroneckerDelta(gt2,gt3);

   return vertex_type(left, right);
}

template<> inline
Vertex<bar<Fe>::type, Fv, conj<Hp>::type>::vertex_type
Vertex<bar<Fe>::type, Fv, conj<Hp>::type>::evaluate(const indices_type& indices, const EvaluationContext& context)
{
   const int gt1 = indices[0];
   const int gt2 = indices[1];
   const auto Ye = MODELPARAMETER(Ye);
   const auto Ue = MODELPARAMETER(Ue);

   const std::complex<double> left = -SUM(j1,0,2,Conj(Ue(gt1,j1))*Ye(j1,gt2));

   const std::complex<double> right = 0;

   return vertex_type(left, right);
}

template<> inline
Vertex<Fe, bar<Fv>::type, Hp>::vertex_type
Vertex<Fe, bar<Fv>::type, Hp>::evaluate(const indices_type& indices, const EvaluationContext& context)
{
   const int gt1 = indices[0];
   const int gt2 = indices[1];
   const auto Ye = MODELPARAMETER(Ye);
   const auto Ue = MODELPARAMETER(Ue);

   const std::complex<double> left = 0;

   const std::complex<double> right = -SUM(j1,0,2,Conj(Ye(j1,gt2))*Ue(gt1,j1));

   return vertex_type(left, right);
}

template<> inline
Vertex<VP, Hp, conj<Hp>::type>::vertex_type
Vertex<VP, Hp, conj<Hp>::type>::evaluate(const indices_type& indices, const EvaluationContext& context)
{
   const auto g1 = MODELPARAMETER(g1);
   const auto g2 = MODELPARAMETER(g2);
   const auto ThetaW = DERIVEDPARAMETER(ThetaW);

   const std::complex<double> result = 0.5*(0.7745966692414834*g1*Cos(ThetaW) + g2*Sin(ThetaW));

   return vertex_type(result);
}

template<> inline
Vertex<bar<Fe>::type, hh, Fe>::vertex_type
Vertex<bar<Fe>::type, hh, Fe>::evaluate(const indices_type& indices, const EvaluationContext& context)
{
   const int gt1 = indices[0];
   const int gt3 = indices[1];
   const auto Ye = MODELPARAMETER(Ye);
   const auto Ve = MODELPARAMETER(Ve);
   const auto Ue = MODELPARAMETER(Ue);

   const std::complex<double> left = -0.7071067811865475*SUM(j2,0,2,Conj(Ve(gt3,j2))*SUM(j1,0,2,Conj(Ue(gt1,j1))*Ye(j1,j2)));

   const std::complex<double> right = -0.7071067811865475*SUM(j2,0,2,SUM(j1,0,2,Conj(Ye(j1,j2))*Ue(gt3,j1))*Ve(gt1,j2));

   return vertex_type(left, right);
}

template<> inline
Vertex<Fe, hh, bar<Fe>::type>::vertex_type
Vertex<Fe, hh, bar<Fe>::type>::evaluate(const indices_type& indices, const EvaluationContext& context)
{
   const int gt1 = indices[0];
   const int gt3 = indices[1];
   const auto Ye = MODELPARAMETER(Ye);
   const auto Ve = MODELPARAMETER(Ve);
   const auto Ue = MODELPARAMETER(Ue);

   const std::complex<double> left = -0.7071067811865475*SUM(j2,0,2,Conj(Ve(gt1,j2))*SUM(j1,0,2,Conj(Ue(gt3,j1))*Ye(j1,j2)));

   const std::complex<double> right = -0.7071067811865475*SUM(j2,0,2,SUM(j1,0,2,Conj(Ye(j1,j2))*Ue(gt1,j1))*Ve(gt3,j2));

   return vertex_type(left, right);
}


template<> inline
double EvaluationContext::mass_impl<VG>(const std::array<int, 1>& indices) const
{ return model.get_MVG(); }

template<> inline
double EvaluationContext::mass_impl<Hp>(const std::array<int, 0>& indices) const
{ return model.get_MHp(); }

template<> inline
double EvaluationContext::mass_impl<ss>(const std::array<int, 0>& indices) const
{ return model.get_Mss(); }

template<> inline
double EvaluationContext::mass_impl<Fv>(const std::array<int, 1>& indices) const
{ return model.get_MFv(indices[0]); }

template<> inline
double EvaluationContext::mass_impl<Ah>(const std::array<int, 0>& indices) const
{ return model.get_MAh(); }

template<> inline
double EvaluationContext::mass_impl<hh>(const std::array<int, 0>& indices) const
{ return model.get_Mhh(); }

template<> inline
double EvaluationContext::mass_impl<VP>(const std::array<int, 0>& indices) const
{ return model.get_MVP(); }

template<> inline
double EvaluationContext::mass_impl<VZ>(const std::array<int, 0>& indices) const
{ return model.get_MVZ(); }

template<> inline
double EvaluationContext::mass_impl<Fd>(const std::array<int, 2>& indices) const
{ return model.get_MFd(indices[0]); }

template<> inline
double EvaluationContext::mass_impl<Fu>(const std::array<int, 2>& indices) const
{ return model.get_MFu(indices[0]); }

template<> inline
double EvaluationContext::mass_impl<Fe>(const std::array<int, 1>& indices) const
{ return model.get_MFe(indices[0]); }

template<> inline
double EvaluationContext::mass_impl<VWp>(const std::array<int, 0>& indices) const
{ return model.get_MVWp(); }

namespace impl {
   static LeftAndRightComponentedVertex unit_charge(const EvaluationContext& context)
   {
      using vertex_type = LeftAndRightComponentedVertex;

      std::array<int, 1> electron_indices = { 0 };
      std::array<int, 0> photon_indices = {};
      std::array<int, 2> indices = concatenate(concatenate(photon_indices, electron_indices), electron_indices);

         const int gt2 = indices[0];
      const int gt3 = indices[1];
      const auto g1 = MODELPARAMETER(g1);
      const auto g2 = MODELPARAMETER(g2);
      const auto ThetaW = DERIVEDPARAMETER(ThetaW);

      const std::complex<double> left = -0.7745966692414834*g1*Cos(ThetaW)*KroneckerDelta(gt2,gt3);

      const std::complex<double> right = -0.5*KroneckerDelta(gt2,gt3)*(0.7745966692414834*g1*Cos(ThetaW) + g2*Sin(ThetaW));

      return vertex_type(left, right);
   }
}

static double unit_charge(const EvaluationContext& context)
{
   return impl::unit_charge(context).left().real() / Electron::electric_charge;
}

} // namespace cxx_diagrams
} // namespace flexiblesusy

#undef INPUTPARAMETER
#undef MODELPARAMETER
#undef DERIVEDPARAMETER
#undef PHASE

#endif
