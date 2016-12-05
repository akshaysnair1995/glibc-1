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

/* Thread specific storage.  */
tss_t key;

int
print_id (void *val)
{
  tss_create (&key, NULL);
  tss_set (key, (void *) 0xff);
  printf ("Child key: %u\n", key);
  printf ("Child value: %p\n", tss_get (key));


  thrd_exit(thrd_success);
}

int
do_test (void)
{

  thrd_t id;
  int ret_val;

  /* Create new thread.  */
  if (thrd_create (&id, print_id, NULL) != thrd_success)
  {
    printf ("Create failed\n");
    _exit (1);
  }

  if (thrd_join (id, &ret_val) != thrd_success)
  {
    printf ("Join failed\n");
    _exit (1);
  }
  else
    printf ("Joined thread\n");

  /* We check parent has the same key value, but
     no value, because was not assigned.  */
  printf ("Parent key: %u\n", key);
  printf ("Parent value: %p\n", tss_get (key));
  tss_delete (key);

  return 0;
}


#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
