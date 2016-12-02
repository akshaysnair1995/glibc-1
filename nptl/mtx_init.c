/* C11 threads mutex initialization implementation.
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

#include "thrd_priv.h"

/* Creates a new mutex object with type __type.  If successful new object
   is pointed by __mutex.  */
int
mtx_init (mtx_t *mutex, int type)
{
  pthread_mutexattr_t config;

  __pthread_mutexattr_init (&config);

  int pthd_type;
  switch (type)
  {
    case mtx_plain | mtx_recursive:
    case mtx_timed | mtx_recursive:
      pthd_type = PTHREAD_MUTEX_RECURSIVE;
      break;
    case mtx_plain:
    case mtx_timed: /* No difference between both in standard */
    default:
      pthd_type = PTHREAD_MUTEX_DEFAULT;
      break;
  }

  __pthread_mutexattr_settype (&config, pthd_type);
  int err_code = __pthread_mutex_init ((pthread_mutex_t *) mutex, &config);
  return thrd_err_map (err_code);
}
