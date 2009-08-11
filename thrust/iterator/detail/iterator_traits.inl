/*
 *  Copyright 2008-2009 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


/*! \file iterator_traits.inl
 *  \brief Inline file for iterator_traits.h.
 */

#include <thrust/iterator/iterator_categories.h>
#include <thrust/iterator/detail/iterator_category_to_traversal.h>
#include <thrust/detail/type_traits.h>


#if __GNUC__
// forward declaration of gnu's __normal_iterator
namespace __gnu_cxx
{

template<typename Iterator, typename Container> class __normal_iterator;

} // end __gnu_cxx
#endif // __GNUC__

#if _MSC_VER
// forward declaration of MSVC's "normal iterators"
namespace std
{

template<typename Value, typename Difference, typename Pointer, typename Reference> struct _Ranit;

} // end std
#endif // _MSC_VER


namespace thrust
{

namespace experimental
{


template<typename Iterator>
  struct iterator_value
{
  typedef typename thrust::iterator_traits<Iterator>::value_type type;
}; // end iterator_value


template<typename Iterator>
  struct iterator_pointer
{
  typedef typename thrust::iterator_traits<Iterator>::pointer type;
}; // end iterator_pointer


template<typename Iterator>
  struct iterator_reference
{
  typedef typename iterator_traits<Iterator>::reference type;
}; // end iterator_reference


template<typename Iterator>
  struct iterator_difference
{
  typedef typename thrust::iterator_traits<Iterator>::difference_type type;
}; // end iterator_difference


template<typename Iterator>
  struct iterator_space
    : detail::iterator_category_to_space<
        typename thrust::iterator_traits<Iterator>::iterator_category
      >
{
}; // end iterator_space


template <typename Iterator>
  struct iterator_traversal
    : detail::iterator_category_to_traversal<
        typename thrust::iterator_traits<Iterator>::iterator_category
      >
{
}; // end iterator_traversal

namespace detail
{

template <typename T>
  struct is_iterator_traversal
    : thrust::detail::is_convertible<T, incrementable_traversal_tag>
{
}; // end is_iterator_traversal


template<typename T>
  struct is_iterator_space
    : detail::or_<
        detail::is_convertible<T, space::any>,
        detail::or_<
          detail::is_convertible<T, space::host>,
          detail::is_convertible<T, space::device>
        >
      >
{
}; // end is_iterator_space

} // end detail

} // end experimental

namespace detail
{

template <typename Iterator> struct iterator_device_reference {};

template <typename T>
  struct iterator_device_reference<T*>
{
  typedef T& type;
}; // end iterator_device_reference



#ifdef __GNUC__
template<typename T>
  struct is_gnu_normal_iterator
    : false_type
{
}; // end is_gnu_normal_iterator


// catch gnu __normal_iterators
template<typename Iterator, typename Container>
  struct is_gnu_normal_iterator< __gnu_cxx::__normal_iterator<Iterator, Container> >
    : true_type
{
}; // end is_gnu_normal_iterator
#endif // __GNUC__


#ifdef _MSC_VER
// catch msvc _Ranit
template<typename Iterator>
  struct is_convertible_to_msvc_Ranit :
    is_convertible<
      Iterator,
      std::_Ranit<
        typename experimental::iterator_value<Iterator>::type,
        typename experimental::iterator_difference<Iterator>::type,
        typename experimental::iterator_pointer<Iterator>::type,
        typename experimental::iterator_reference<Iterator>::type
      >
    > {};
#endif // _MSC_VER


template<typename T>
  struct is_trivial_iterator :
    integral_constant<
      bool,
        is_pointer<T>::value
      | is_device_ptr<T>::value
#if __GNUC__
      | is_gnu_normal_iterator<T>::value
#endif // __GNUC__
#ifdef _MSC_VER
      | is_convertible_to_msvc_Ranit<T>::value
#endif // _MSC_VER
    > {};

} // end detail

} // end thrust
