/**
 * @file memory.h
 * Definitions for kernel memory allocator and maintenance.
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stddef.h>
#include <spinlock.h>

/* roundmb - round address up to size of memblock  */
#define roundmb(x)  (void *)( (7 + (ulong)(x)) & ~0x07 )
/* truncmb - truncate address down to size of memblock */
#define truncmb(x)  (void *)( ((ulong)(x)) & ~0x07 )

/**
 * Structure for a block of memory.
 */
typedef struct memblock
{
    struct memblock *next;      /* pointer to next memory block       */
    ulong length;               /* size of memory block (with struct) */
} memblk;

/**
 * Structure for the head of the memory list.
 */
typedef struct memhead
{
    struct memblock *head;      /* pointer to first memory block      */
    ulong length;               /* non-constant; current size of list */
    ulong base;                 /* beginning address of free list     */
    ulong bound;                /* fixed value; total size of list    */
    spinlock_t memlock;         /* lock for mutual exclusion          */
} memhead;

extern memhead freelist[];      /* heads of free memory lists         */
                                /* one freelist for each core         */

/* Other memory data */

extern void *_end;              /* linker provides end of image       */
extern void *memheap;           /* bottom of heap                     */

/* Memory function prototypes */
void *getmem(ulong nbytes);
syscall freemem(void *pmem, ulong nbytes);

void *malloc(ulong nbytes);
syscall free(void *pmem);

#endif                          /* _MEMORY_H_ */
