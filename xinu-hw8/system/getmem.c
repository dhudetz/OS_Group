/**
 * @file getmem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Allocate heap memory.
 *
 * @param nbytes
 *      Number of bytes requested.
 *
 * @return
 *      ::SYSERR if @p nbytes was 0 or there is no memory to satisfy the
 *      request; otherwise returns a pointer to the allocated memory region.
 *      The returned pointer is guaranteed to be 8-byte aligned.  Free the block
 *      with memfree() when done with it.
 */
void *getmem(ulong nbytes)
{
    register memblk *prev, *curr, *leftover;
    irqmask im;

    if (0 == nbytes)
    {
        return (void *)SYSERR;
    }

    /* round to multiple of memblock size   */
    nbytes = (ulong)roundmb(nbytes);

    im = disable();
    int cpuid = getcpuid();
    register memhead mhead = freelist[cpuid];
    lock_acquire(mhead.memlock);
    prev = mhead.head;
    curr = prev->next;
    ulong newbytes = nbytes;
    if(prev->length >= nbytes && nbytes > prev->length - 8){
	mhead.head = curr;
	freelist[cpuid].length += nbytes;
        lock_release(mhead.memlock);
	return prev;
    }
    else if(prev->length-8 >= nbytes){
	while(newbytes%8 != 0)
	    newbytes++;
	leftover = (ulong)prev + newbytes;
	leftover->next = curr;
	leftover->length = prev->length - newbytes;
	freelist[cpuid].head = leftover;
	prev->length = newbytes;
	freelist[cpuid].length -= nbytes;
        lock_release(mhead.memlock);
	return prev;
    }
    else{ 
        while(curr){
            if(curr->length >= nbytes && nbytes > curr->length - 8){
    	        prev->next = curr->next;
		freelist[cpuid].length += nbytes;
    		lock_release(mhead.memlock);
                return curr;
	    }
	    else{
	        while(newbytes%8 != 0)
		    newbytes++;
		leftover = (struct memblock *)((ulong)curr + newbytes);
		leftover->next = curr->next;
		leftover->length = curr->length - newbytes;
		prev->next = leftover;
		curr->length = newbytes;
		freelist[cpuid].length -= nbytes;
    		lock_release(mhead.memlock);
		return curr;	
	    }
    	    prev = curr;
            curr = curr->next;
	}
    }
    lock_release(mhead.memlock);
	/* TODO:
     *      - Use cpuid to use correct freelist
     *           ex: freelist[cpuid]
     *      - Acquire memory lock (memlock)
     *      - Traverse through the freelist
     *        to find a block that's suitable 
     *        (Use First Fit with simple compaction)
     *      - Release memory lock
     *      - return memory address if successful
     */

    restore(im);
    return (void *)SYSERR;
}
