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

/* Shared mutex between child and parent.  */
static mtx_t mutex;

/* Shared counter to check possible race conditions.  */
static char shrd_counter;

/* Maximum amount of time waiting for mutex.  */
static struct timespec wait_time;

/* Function to choose an action to do, depending on mtx_timedlock
   return value.  */
static inline void
choose_action (int action, char* thread_name)
{
  switch (action)
  {
    case thrd_success:

      /* Critical section.  */
      printf ("%s locking.\n", thread_name);
      ++shrd_counter;
      printf ("%s unlocking.\n", thread_name);

      /* Unlock mutex.  */
      if (mtx_unlock (&mutex) != thrd_success)
      {
        printf ("%s unlock error.\n", thread_name);
        _exit (1);
      }
      break;

    case thrd_timedout:
      printf ("Waiting for mutex too long, %s leaving.\n", thread_name);
      break;

    case thrd_error:
      printf ("%s lock error.\n", thread_name);
      _exit (1);
      break;
  }

}

int
child_add (void)
{
  char child_name[] = "Child";

  /* Try to lock mutex.  */
  choose_action (mtx_timedlock (&mutex, &wait_time), child_name);
  thrd_exit (thrd_success);
}

int
do_test (void)
{
  thrd_t id;
  char parent_name[] = "Parent";

  /* Initialize mutex.  */
  mtx_init (&mutex, mtx_timed);

  /* Initialize waiting time.  */
  clock_gettime (CLOCK_REALTIME, &wait_time);
  /* Tiny amount of time, to assure that if any thread
     finds it busy, will receive thrd_timedout.  */
  wait_time.tv_nsec += 1;



  /* Create new thread.  */
  if (thrd_create (&id, (thrd_start_t) child_add, NULL) != thrd_success)
  {
    printf ("Create failed\n");
    _exit (1);
  }

  /* Try to lock mutex.  */
  choose_action (mtx_timedlock (&mutex, &wait_time), parent_name);

  /* Still have to wait until child is done.  */
  if (thrd_join (id, NULL) != thrd_success)
  {
    printf ("Join failed.\n");
    _exit (1);
  }

  /* Check result.  */
  if (shrd_counter != 2 && shrd_counter != 1)
  {
    printf ("Counter has a wrong value: %i.\n", shrd_counter);
    _exit (1);
  }

  /* Destroy mutex.  */
  mtx_destroy (&mutex);

  return 0;
}


#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
