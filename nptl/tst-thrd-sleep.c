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
#include <time.h>
#include <stdio.h>
#include <unistd.h>


int
go_to_sleep (struct timespec const *tl)
{

  printf ("Time before sleeping: %s", ctime (&(time_t){time(NULL)}));
  if (thrd_sleep (tl, NULL) != 0)
  {
    printf ("Sleep failed\n");
    _exit (1);
  }
  printf ("Time after sleeping: %s", ctime (&(time_t){time(NULL)}));

  return 0;
}

int
do_test (void)
{

  thrd_t id;
  /* Wait for 3 seconds.  */
  struct timespec wait_time = {.tv_sec = 3};

  /* Create new thread.  */
  if (thrd_create (&id, (thrd_start_t) go_to_sleep,
        (void *) (&wait_time)) != thrd_success)
  {
    printf ("Create failed\n");
    _exit (1);
  }
  else
    printf ("Created thread\n");


  if (thrd_join (id, NULL) != thrd_success)
  {
    printf ("Join failed\n");
    _exit (1);
  }
  else
    printf ("Joined thread\n");

  return 0;
}


#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
