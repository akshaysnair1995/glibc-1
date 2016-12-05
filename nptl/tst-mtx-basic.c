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

int
child_add (void)
{
  /* Lock mutex.  */
  if (mtx_lock (&mutex) != thrd_success)
  {
    printf ("Child lock error.\n");
    _exit (1);
  }

  /* Critical section.  */
  printf ("Child locking\n");
  ++shrd_counter;
  printf ("Child unlocking\n");

  /* Unlock mutex.  */
  if (mtx_unlock (&mutex) != thrd_success)
  {
    printf ("Child unlock error.\n");
    _exit (1);
  }
  thrd_exit (thrd_success);
}

int
do_test (void)
{

  thrd_t id;

  /* Initialize mutex.  */
  mtx_init (&mutex, mtx_plain);

  /* Create new thread.  */
  if (thrd_create (&id, (thrd_start_t) child_add, NULL) != thrd_success)
  {
    printf ("Create failed\n");
    _exit (1);
  }
  else
    printf ("Created thread\n");


  if (mtx_lock (&mutex) != thrd_success)
  {
    printf ("Parent lock failed\n");
    _exit (1);
  }

  /* Critical section.  */
  printf ("Parent locking\n");
  ++shrd_counter;
  printf ("Parent unlocking\n");

  /* Unlock mutex.  */
  if (mtx_unlock (&mutex) != thrd_success)
  {
    printf ("Parent unlock error.\n");
    _exit (1);
  }


  /* Still have to wait until child is done.  */
  if (thrd_join (id, NULL) != thrd_success)
  {
    printf ("Join failed.\n");
    _exit (1);
  }

  /* Check result.  */
  if (shrd_counter != 2)
  {
    printf ("Counter has a wrong value.\n");
    _exit (1);
  }

  /* Destroy mutex.  */
  mtx_destroy (&mutex);

  return 0;
}


#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
