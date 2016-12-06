/* Copyright (C) 2005-2016 Free Software Foundation, Inc.
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
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _BITS_PTHREADTYPES_H
#define _BITS_PTHREADTYPES_H	1

#include <bits/pthreadtypes-common.h>

/* Linuxthread type sizes (bytes):
   sizeof(pthread_attr_t) = 0x24 (36)
   sizeof(pthread_barrier_t) = 0x30 (48)
   sizeof(pthread_barrierattr_t) = 0x4 (4)
   sizeof(pthread_cond_t) = 0x30 (48)
   sizeof(pthread_condattr_t) = 0x4 (4)
   sizeof(pthread_mutex_t) = 0x30 (48)
   sizeof(pthread_mutexattr_t) = 0x4 (4)
   sizeof(pthread_rwlock_t) = 0x40 (64)
   sizeof(pthread_rwlockattr_t) = 0x8 (8)
   sizeof(pthread_spinlock_t) = 0x10 (16) */

#define __SIZEOF_PTHREAD_ATTR_T 36
#define __SIZEOF_PTHREAD_BARRIER_T 48
#define __SIZEOF_PTHREAD_BARRIERATTR_T 4
#define __SIZEOF_PTHREAD_CONDATTR_T 4
#define __SIZEOF_PTHREAD_MUTEXATTR_T 4
#define __SIZEOF_PTHREAD_RWLOCK_T 64
#define __SIZEOF_PTHREAD_RWLOCKATTR_T 8

/* Thread identifiers.  The structure of the attribute type is not
   exposed on purpose.  */
typedef unsigned long int pthread_t;

union pthread_attr_t
{
  char __size[__SIZEOF_PTHREAD_ATTR_T];
  long int __align;
};
#ifndef __have_pthread_attr_t
typedef union pthread_attr_t pthread_attr_t;
# define __have_pthread_attr_t	1
#endif


typedef union
{
  char __size[__SIZEOF_PTHREAD_MUTEXATTR_T];
  long int __align;
} pthread_mutexattr_t;


typedef union
{
  char __size[__SIZEOF_PTHREAD_CONDATTR_T];
  long int __align;
} pthread_condattr_t;


/* Keys for thread-specific data */
typedef unsigned int pthread_key_t;


/* Once-only execution */
typedef int pthread_once_t;


#if defined __USE_UNIX98 || defined __USE_XOPEN2K
/* Data structure for read-write lock variable handling.  The
   structure of the attribute type is not exposed on purpose.  */
typedef union
{
  struct
  {
    /* In the old Linuxthreads pthread_rwlock_t, this is the
       start of the 4-word 16-byte aligned lock structure. The
       next four words are all set to 1 by the Linuxthreads
       PTHREAD_RWLOCK_INITIALIZER. We ignore them in NPTL.  */
    int __compat_padding[4] __attribute__ ((__aligned__(16)));
    int __lock;
    unsigned int __nr_readers;
    unsigned int __readers_wakeup;
    unsigned int __writer_wakeup;
    unsigned int __nr_readers_queued;
    unsigned int __nr_writers_queued;
    int __writer;
    /* An unused word, reserved for future use. It was added
       to maintain the location of the flags from the Linuxthreads
       layout of this structure.  */
    int __reserved1;
    /* FLAGS must stay at this position in the structure to maintain
       binary compatibility.  */
    unsigned char __pad2;
    unsigned char __pad1;
    unsigned char __shared;
    unsigned char __flags;
    /* The NPTL pthread_rwlock_t is 4 words smaller than the
       Linuxthreads version. One word is in the middle of the
       structure, the other three are at the end.  */
    int __reserved2;
    int __reserved3;
    int __reserved4;
  } __data;
  char __size[__SIZEOF_PTHREAD_RWLOCK_T];
  long int __align;
} pthread_rwlock_t;

#define __PTHREAD_RWLOCK_ELISION_EXTRA 0

typedef union
{
  char __size[__SIZEOF_PTHREAD_RWLOCKATTR_T];
  long int __align;
} pthread_rwlockattr_t;
#endif


#ifdef __USE_XOPEN2K
/* POSIX spinlock data type.  */
typedef volatile int pthread_spinlock_t;


/* POSIX barriers data type.  The structure of the type is
   deliberately not exposed.  */
typedef union
{
  char __size[__SIZEOF_PTHREAD_BARRIER_T];
  long int __align;
} pthread_barrier_t;

typedef union
{
  char __size[__SIZEOF_PTHREAD_BARRIERATTR_T];
  int __align;
} pthread_barrierattr_t;
#endif


#endif	/* bits/pthreadtypes.h */
