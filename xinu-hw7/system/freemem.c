/**
 * @file freemem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Frees a block of heap-allocated memory.
 *
 * @param memptr
 *      Pointer to memory block allocated with memget().
 *
 * @param nbytes
 *      Length of memory block, in bytes.  (Same value passed to memget().)
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail
 *      because of memory corruption or specifying an invalid memory block.
 */
syscall freemem(void *memptr, ulong nbytes)
{
    register struct memblock *block, *next, *prev;
    irqmask im;
    ulong top;

    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < (ulong)memheap)
        || ((ulong)memptr > (ulong)platform.maxaddr))
    {
        return SYSERR;
    }

    block = (struct memblock *)memptr;
    nbytes = (ulong)roundmb(nbytes);

    im = disable();
    

    register memhead mhead;
    int core;
    ulong mem = (ulong)memptr;
    for(core = 0; core<NCORES; core++){
        mhead = freelist[core];
	ulong lastblock = mhead.base + mhead.bound;
	if(mhead.base <= mem && lastblock > mem)
	    break;
    }
    lock_acquire(mhead.memlock);
    prev = mhead.head;
    next = prev->next;
    if(mhead.base == mem){
        if(mem+nbytes > (ulong)prev)
 	    return SYSERR;
	block->length = nbytes;
	block->next = mhead.head;
	freelist[core].head = block;
    }
    else{
        while(mem<(ulong)prev + prev->length){
	    prev = prev->next;
            next = next->next;
	}
        if(mem+nbytes > (ulong)next)
	    return SYSERR;
	else if(mem < (ulong)prev + prev->length)
	    return SYSERR;	
	prev->next = block;
	block->next = next;
    }
    lock_release(mhead.memlock);
         /* TODO:
     *      - Determine correct freelist to return to
     *        based on block address
     *      - Acquire memory lock (memlock)
     *      - Find where the memory block should
     *        go back onto the freelist (based on address)
     *      - Find top of previous memblock
     *      - Make sure block is not overlapping on prev or next blocks
     *      - Coalesce with previous block if adjacent
     *      - Coalesce with next block if adjacent
     */

    restore(im);
    return OK;
}
