/* string-fzi.h -- zero byte detection; indexes.  Generic C version.
   Copyright (C) 2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef STRING_FZI_H
#define STRING_FZI_H 1

#include <limits.h>
#include <endian.h>
#include <string-fza.h>

/* A subroutine for the index_zero functions.  Given a test word C, return
   the (memory order) index of the first byte (in memory order) that is
   non-zero.  */

static inline unsigned int
index_first_ (op_t c)
{
  _Static_assert (sizeof (op_t) == sizeof (long)
		  || sizeof (op_t) == sizeof (long long),
		  "Unhandled word size");

  unsigned r;
  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    {
      if (sizeof (op_t) == sizeof (long))
	r = __builtin_ctzl (c);
      else
	r = __builtin_ctzll (c);
    }
  else
    {
      if (sizeof (op_t) == sizeof (long))
	r = __builtin_clzl (c);
      else
	r = __builtin_clzll (c);
    }
  return r / CHAR_BIT;
}

/* Similarly, but return the (memory order) index of the last byte
   that is non-zero.  */

static inline unsigned int
index_last_ (op_t c)
{
  _Static_assert (sizeof (op_t) == sizeof (long)
		  || sizeof (op_t) == sizeof (long long),
		  "Unhandled word size");

  unsigned r;
  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    {
      if (sizeof (op_t) == sizeof (long))
	r = __builtin_clzl (c);
      else
	r = __builtin_clzll (c);
    }
  else
    {
      if (sizeof (op_t) == sizeof (long))
	r = __builtin_ctzl (c);
      else
	r = __builtin_ctzll (c);
    }
  return sizeof (op_t) - 1 - (r / CHAR_BIT);
}

/* Given a word X that is known to contain a zero byte, return the
   index of the first such within the word in memory order.  */

static inline unsigned int
index_first_zero (op_t x)
{
  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    x = find_zero_low (x);
  else
    x = find_zero_all (x);
  return index_first_ (x);
}

/* Similarly, but perform the search for byte equality between X1 and X2.  */

static inline unsigned int
index_first_eq (op_t x1, op_t x2)
{
  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    x1 = find_eq_low (x1, x2);
  else
    x1 = find_eq_all (x1, x2);
  return index_first_ (x1);
}

/* Similarly, but perform the search for zero within X1 or
   equality between X1 and X2.  */

static inline unsigned int
index_first_zero_eq (op_t x1, op_t x2)
{
  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    x1 = find_zero_eq_low (x1, x2);
  else
    x1 = find_zero_eq_all (x1, x2);
  return index_first_ (x1);
}

/* Similarly, but perform the search for zero within X1 or
   inequality between X1 and X2.  */

static inline unsigned int
index_first_zero_ne (op_t x1, op_t x2)
{
  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    x1 = find_zero_ne_low (x1, x2);
  else
    x1 = find_zero_ne_all (x1, x2);
  return index_first_ (x1);
}

/* Similarly, but search for the last zero within X.  */

static inline unsigned int
index_last_zero (op_t x)
{
  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    x = find_zero_all (x);
  else
    x = find_zero_low (x);
  return index_last_ (x);
}

static inline unsigned int
index_last_eq (op_t x1, op_t x2)
{
  return index_last_zero (x1 ^ x2);
}

#endif /* STRING_FZI_H */
