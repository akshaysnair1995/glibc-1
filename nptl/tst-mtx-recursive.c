/* Copyright (C) 2015 Free Software Foundation, Inc.
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

#include <threads.h>
#include <stdio.h>
#include <unistd.h>


int
do_test (void)
{

  static mtx_t mutex;

  /* Initialize mutex.  */
  mtx_init (&mutex, mtx_recursive);

  /* Lock mutex first time.  */
  if (mtx_lock (&mutex) != thrd_success)
  {
    printf ("First lock failed\n");
    _exit (1);
  }
  else
    printf ("Mutex locked once\n");

  /* Lock mutex second time, if not recursive should deadlock.  */
  if (mtx_lock (&mutex) != thrd_success)
  {
    printf ("Second lock failed\n");
    _exit (1);
  }
  else
    printf ("Mutex locked twice\n");

  /* Destroy mutex.  */
  mtx_destroy (&mutex);

  return 0;
}

#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
