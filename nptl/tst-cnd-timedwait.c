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

/* Shared condition variable between child and parent.  */
static cnd_t cond;

/* Mutex needed to signal and wait threads.  */
static mtx_t mutex;

int
signal_parent (void)
{
  /* Send signal to parent.  */
  printf ("Child signaling\n");
  if (cnd_signal (&cond) != thrd_success)
  {
    printf ("Child signal error.\n");
    _exit (1);
  }

  //thrd_exit(thrd_success);
  return 0;
}

int
do_test (void)
{

  thrd_t id;
  struct timespec w_time;

  /* Initialize cond and mutex.  */
  if (cnd_init (&cond) != thrd_success
      || mtx_init (&mutex, mtx_timed) != thrd_success)
  {
    printf ("Create failed\n");
    _exit (1);
  }

  /* Initialize waiting time.  */
  clock_gettime (CLOCK_REALTIME, &w_time);
  w_time.tv_nsec += 150000;

  /* Create new thread.  */
  if (thrd_create (&id, (thrd_start_t) signal_parent, NULL)
       != thrd_success)
  {
    printf ("Create failed\n");
    _exit (1);
  }

  /* Wait on cond, until child thread signals.  */
  switch (cnd_timedwait (&cond, &mutex, &w_time))
  {
    case thrd_error:
      printf ("Parent lock failed\n");
      _exit (1);
    case thrd_timedout:
      printf ("Waiting too long, leaving\n");
      break;
    case thrd_success:
      printf ("Signaled successfully,\n");
      break;
  }

  /* Joining is not mandatory here, but still done
     to assure child thread ends correctly..  */
  if (thrd_join (id, NULL) != thrd_success)
  {
    printf ("Join failed.\n");
    _exit (1);
  }


  /* Destroy mutex.  */
  mtx_destroy (&mutex);

  /* Destroy cond.  */
  cnd_destroy (&cond);

  return 0;
}


#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
