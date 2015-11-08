/* Implementation of the POSIX sleep function using nanosleep.
   Copyright (C) 1996-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1996.

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

#include <time.h>
#include <unistd.h>
#include <sysdep-cancel.h>

unsigned int
__sleep (unsigned int seconds)
{
  struct timespec ts = { .tv_sec = seconds, .tv_nsec = 0 };
  if (__nanosleep (&ts, &ts))
    return ts.tv_sec;
  return 0;
}
weak_alias (__sleep, sleep)
