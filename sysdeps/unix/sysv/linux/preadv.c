/* Copyright (C) 2009-2015 Free Software Foundation, Inc.
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

#include <errno.h>
#include <stddef.h>
#include <sys/param.h>
#include <stdint.h>
#include <sys/uio.h>

#include <sysdep-cancel.h>
#include <sys/syscall.h>
#include <kernel-features.h>


#ifndef PREADV
# define PREADV preadv
# define PREADV_REPLACEMENT __atomic_preadv_replacement
# define PREAD __pread
# define OFF_T off_t
# define OFF_ARG(__x) __SYSCALL_LL_O (__x)
#endif

#ifndef __ASSUME_PREADV
static ssize_t PREADV_REPLACEMENT (int, const struct iovec *,
				   int, OFF_T) internal_function;
#endif


ssize_t
PREADV (int fd, const struct iovec *vector, int count, OFF_T offset)
{
#ifdef __NR_preadv
  ssize_t result;

  result = SYSCALL_CANCEL (preadv, fd, vector, count,
			   __ALIGNMENT_ARG OFF_ARG (offset));

# ifdef __ASSUME_PREADV
  return result;
# endif
#endif

#ifndef __ASSUME_PREADV
# ifdef __NR_preadv
  if (result >= 0 || errno != ENOSYS)
    return result;
# endif

  return PREADV_REPLACEMENT (fd, vector, count, offset);
#endif
}

#ifndef __ASSUME_PREADV
# undef PREADV
# define PREADV static internal_function PREADV_REPLACEMENT
# include <sysdeps/posix/preadv.c>
#endif
