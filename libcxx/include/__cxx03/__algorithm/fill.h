//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___CXX03___ALGORITHM_FILL_H
#define _LIBCPP___CXX03___ALGORITHM_FILL_H

#include <__cxx03/__algorithm/fill_n.h>
#include <__cxx03/__config>
#include <__cxx03/__iterator/iterator_traits.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

// fill isn't specialized for std::memset, because the compiler already optimizes the loop to a call to std::memset.

template <class _ForwardIterator, class _Tp>
inline _LIBCPP_HIDE_FROM_ABI void
__fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value, forward_iterator_tag) {
  for (; __first != __last; ++__first)
    *__first = __value;
}

template <class _RandomAccessIterator, class _Tp>
inline _LIBCPP_HIDE_FROM_ABI void
__fill(_RandomAccessIterator __first, _RandomAccessIterator __last, const _Tp& __value, random_access_iterator_tag) {
  std::fill_n(__first, __last - __first, __value);
}

template <class _ForwardIterator, class _Tp>
inline _LIBCPP_HIDE_FROM_ABI void fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) {
  std::__fill(__first, __last, __value, typename iterator_traits<_ForwardIterator>::iterator_category());
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___CXX03___ALGORITHM_FILL_H
