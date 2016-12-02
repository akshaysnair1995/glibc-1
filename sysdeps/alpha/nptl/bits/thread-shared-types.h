/* Common thread definition for pthread and c11 threads. Linux/alpha
   version.
   Copyright (C) 2016 Free Software Foundation, Inc.
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

/* This header contains macros definition required for the declaration of
   pthread_mutex_t and pthread_cond_t, both needed by C11 types mtx_t and
   cnd_t. */

#ifndef _BITS_PTHREAD_SHARED_TYPES_H
# define _BITS_PTHREAD_SHARED_TYPES_H	1

#define __SIZEOF_PTHREAD_MUTEX_T 40
#define __SIZEOF_PTHREAD_COND_T 48


typedef struct __pthread_internal_list
{
  struct __pthread_internal_list *__prev;
  struct __pthread_internal_list *__next;
} __pthread_list_t;


struct __pthread_mutex_s
{
  int __lock;
  unsigned int __count;
  int __owner;
  unsigned int __nusers;
  /* KIND must stay at this position in the structure to maintain
     binary compatibility with static initializers.  */
  int __kind;
  int __spins;
  __pthread_list_t __list;
#define __PTHREAD_MUTEX_HAVE_PREV	1
};

/* Mutex __spins initializer used by PTHREAD_MUTEX_INITIALIZER.  */
#define __PTHREAD_SPINS 0

#define __PTHREAD_MUTEX_T_CONTENT		\
  struct __pthread_mutex_s  __data;		\
  char __size[__SIZEOF_PTHREAD_MUTEX_T];	\
  long int __align;


/* Data structure for conditional variable handling */
#define __PTHREAD_COND_T_CONTENT			\
  struct						\
  {							\
    int __lock;						\
    unsigned int __futex;				\
    __extension__ unsigned long long int __total_seq;	\
    __extension__ unsigned long long int __wakeup_seq;	\
    __extension__ unsigned long long int __woken_seq;	\
    void *__mutex;					\
    unsigned int __nwaiters;				\
    unsigned int __broadcast_seq;			\
  } __data;						\
  char __size[__SIZEOF_PTHREAD_COND_T];			\
  __extension__ long long int __align;

#endif
