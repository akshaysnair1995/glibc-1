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

#ifndef _BITS_PTHREAD_SHARED_TYPES_H
#define _BITS_PTHREAD_SHARED_TYPES_H	1

#ifdef __x86_64__
# if __WORDSIZE == 64
#  define __SIZEOF_PTHREAD_MUTEX_T 40
# else
#  define __SIZEOF_PTHREAD_MUTEX_T 32
# endif
#else
# define __SIZEOF_PTHREAD_MUTEX_T 24
#endif
#define __SIZEOF_PTHREAD_COND_T 48

#ifdef __x86_64__
typedef struct __pthread_internal_list
{
  struct __pthread_internal_list *__prev;
  struct __pthread_internal_list *__next;
} __pthread_list_t;
#else
typedef struct __pthread_internal_slist
{
  struct __pthread_internal_slist *__next;
} __pthread_slist_t;
#endif


struct __pthread_mutex_s
{
  int __lock;
  unsigned int __count;
  int __owner;
#ifdef __x86_64__
  unsigned int __nusers;
#endif
  /* KIND must stay at this position in the structure to maintain
     binary compatibility with static initializers.  */
  int __kind;
#ifdef __x86_64__
  short __spins;
  short __elision;
  __pthread_list_t __list;
# define __PTHREAD_MUTEX_HAVE_PREV	1
/* Mutex __spins initializer used by PTHREAD_MUTEX_INITIALIZER.  */
# define __PTHREAD_SPINS             0, 0
#else
  unsigned int __nusers;
  __extension__ union
  {
    struct
    {
      short __espins;
      short __elision;
# define __spins __elision_data.__espins
# define __elision __elision_data.__elision
# define __PTHREAD_SPINS         { 0, 0 }
    } __elision_data;
    __pthread_slist_t __list;
  };
#endif
};

#define __PTHREAD_MUTEX_T_CONTENT		\
  struct __pthread_mutex_s __data;		\
  char __size[__SIZEOF_PTHREAD_MUTEX_T];	\
  long int __align;				\

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
