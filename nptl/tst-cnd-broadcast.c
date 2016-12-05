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

/* Condition variable where child threads will wait.  */
cnd_t cond;

/* Mutex to control wait on cond.  */
mtx_t mutex;

/* Code executed by each thread.  */
int
child_wait (void* data)
{
  printf ("Arrived\n");
  /* Wait until parent thread sends broadcast here.  */
  mtx_lock (&mutex);
  cnd_wait (&cond, &mutex);
  mtx_unlock (&mutex);

  printf ("Freed thread\n");
  thrd_exit (thrd_success);
}

#define N 5

int
do_test (void)
{

  thrd_t ids[N];
  unsigned char i;

  cnd_init (&cond);
  mtx_init (&mutex, mtx_plain);

  /* Create N new threads.  */
  for (i = 0; i < N; ++i)
  {
    if (thrd_create (&ids[i], child_wait, NULL) != thrd_success)
    {
      printf ("Create failed\n");
      _exit (1);
    }
  }

  /* Wait for other threads to reach their wait func.  */
  thrd_sleep (&((struct timespec){.tv_sec = 2}), NULL);

  /* Send broadcast.  */
  mtx_lock (&mutex);
  if (cnd_broadcast (&cond) != thrd_success)
  {
    printf ("Broadcast error");
    _exit (1);
  }
  mtx_unlock (&mutex);

  /* Join threads.  */
  for (i = 0; i < N; ++i)
  {
    if (thrd_join (ids[i], NULL) != thrd_success)
    {
      printf ("Join failed\n");
      _exit (1);
    }
  }


  mtx_destroy (&mutex);
  cnd_destroy (&cond);

  return 0;
}


#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
