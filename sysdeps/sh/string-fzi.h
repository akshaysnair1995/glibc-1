/* string-fzi.h -- zero byte detection; indexes.  SH4 version.
   Copyright (C) 2017 Free Software Foundation, Inc.
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

/* A subroutine for the index_zero functions.  Given a test word C, return
   the (memory order) index of the first byte (in memory order) that is
   non-zero.  */

/* Given a word X that is known to contain a zero byte, return the
   index of the first such within the word in memory order.  */

static inline unsigned int
index_first_zero (op_t x)
{
  _Static_assert (sizeof (op_t) == 4, "64-bit not supported");

  /* Since we have no clz insn, direct tests of the bytes is faster
     than loading up the constants to do the masking.  */
  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    { 
      if ((x & 0x000000FF) == 0)
        return 0;
      else if ((x & 0x0000FF00) == 0)
        return 1;
      else if ((x & 0x00FF0000) == 0)
        return 2;
      return 3;
    }
  else
    { 
      if ((x >> 24) == 0)
        return 0;
      else if ((x & 0x00FF0000) == 0)
        return 1;
      else if ((x & 0x0000FF00) == 0)
        return 2;
      return 3;
    }
}

/* Similarly, but perform the search for byte equality between X1 and X2.  */

static inline unsigned int
index_first_eq (op_t x1, op_t x2)
{
  return index_first_zero (x1 ^ x2);
}

/* Similarly, but perform the search for zero within X1 or
   equality between X1 and X2.  */

static inline unsigned int
index_first_zero_eq (op_t x1, op_t x2)
{
  _Static_assert (sizeof (op_t) == 4, "64-bit not supported");

  /* Since we have no clz insn, direct tests of the bytes is faster
     than loading up the constants to do the masking.  */
  op_t xeq = x1 ^ x2;
  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    { 
      if (((x1 & 0x000000FF) == 0) || (xeq & 0x000000FF) == 0)
        return 0;
      else if (((x1 & 0x0000FF00) == 0) || (xeq & 0x0000FF00) == 0)
        return 1;
      else if (((x1 & 0x00FF0000) == 0) || (xeq & 0x00FF0000) == 0)
        return 2;
      return 3;
    }
  else
    { 
      if (((x1 >> 24) == 0) || (xeq >> 24) == 0)
        return 0;
      else if (((x1 & 0x00FF0000) == 0) || (xeq & 0x00FF0000) == 0)
        return 1;
      else if (((x1 & 0x0000FF00) == 0) || (xeq & 0x0000FF00) == 0)
        return 2;
      return 3;
    }
}

/* Similarly, but perform the search for zero within X1 or
   inequality between X1 and X2.  */

static inline unsigned int
index_first_zero_ne (op_t x1, op_t x2)
{
  _Static_assert (sizeof (op_t) == 4, "64-bit not supported");

  /* Since we have no clz insn, direct tests of the bytes is faster
     than loading up the constants to do the masking.  */
  op_t xeq = x1 ^ x2;
  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    { 
      if (((x1 & 0x000000FF) == 0) || (xeq & 0x000000FF) != 0)
        return 0;
      else if (((x1 & 0x0000FF00) == 0) || (xeq & 0x0000FF00) != 0)
        return 1;
      else if (((x1 & 0x00FF0000) == 0) || (xeq & 0x00FF0000) != 0)
        return 2;
      return 3;
    }
  else
    { 
      if (((x1 >> 24) == 0) || (xeq >> 24) != 0)
        return 0;
      else if (((x1 & 0x00FF0000) == 0) || (xeq & 0x00FF0000) != 0)
        return 1;
      else if (((x1 & 0x0000FF00) == 0) || (xeq & 0x0000FF00) != 0)
        return 2;
      return 3;
    }
}

/* Similarly, but search for the last zero within X.  */

static inline unsigned int
index_last_zero (op_t x)
{
  _Static_assert (sizeof (op_t) == 4, "64-bit not supported");

  /* Since we have no clz insn, direct tests of the bytes is faster
     than loading up the constants to do the masking.  */
  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    { 
      if ((x >> 24) == 0)
        return 3;
      else if ((x & 0x00FF0000) == 0)
        return 2;
      else if ((x & 0x0000FF00) == 0)
        return 1;
      return 0;
    }
  else
    { 
      if ((x & 0x000000FF) == 0)
        return 3;
      else if ((x & 0x0000FF00) == 0)
        return 2;
      else if ((x & 0x00FF0000) == 0)
        return 1;
      return 0;
    }
}

static inline unsigned int
index_last_eq (op_t x1, op_t x2)
{
  return index_last_zero (x1 ^ x2);
}

#endif /* STRING_FZI_H */
