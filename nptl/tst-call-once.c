/* Copyright (C) 2016 Free Software Foundation, Inc.
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


/* Flag that controls the first thread access.  */
static once_flag flag = ONCE_FLAG_INIT;

/* Function called only once.  */
void
do_once (void)
{
  printf ("Called once\n");
}

/* Code executed by each thread.  */
int
func (void* data)
{
  call_once (&flag, do_once);
  thrd_exit(thrd_success);
}


#define N 20

int
do_test (void)
{

  thrd_t ids[N];
  unsigned char i;


  /* Create N new threads.  */
  for (i = 0; i < N; ++i)
  {
    if (thrd_create (&ids[i], func, NULL) != thrd_success)
    {
      printf ("Create failed\n");
      _exit (1);
    }
  }

  /* Join threads.  */
  for (i = 0; i < N; ++i)
  {
    if (thrd_join (ids[i], NULL) != thrd_success)
    {
      printf ("Join failed\n");
      _exit (1);
    }
  }

  return 0;
}


#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
