/**
 * @file signaln.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup semaphores
 *
 * Signal a semaphore @p count times, releasing @p count waiting threads.
 *
 * signaln() may reschedule the currently running thread.  As a result,
 * signaln() should not be called from non-reentrant interrupt handlers unless
 * ::resdefer is set to a positive value at the start of the interrupt handler.
 *
 * @param sem
 *      Semaphore to signal.
 * @param count
 *      Number of times to signal, which must be positive.
 *
 * @return
 *      ::OK on success, ::SYSERR on failure.  This function can only fail if @p
 *      sem did not specify a valid semaphore of if @p count was not positive.
 */
syscall signaln(semaphore sem, int count)
{
    register struct sement *semptr;
    int c, d;
    irqmask im;

    im = disable();
    if (isbadsem(sem) || (count <= 0))
    {
        restore(im);
        return SYSERR;
    }
    semptr = &semtab[sem];

    for (; count > 0; count--)
    {
       /* TODO:
	* Make this incrementation atomically-safe
	* so that multiple cores may safely signal a semaphore.
	*/
		c = ++semptr->count;

        if (c <= 0)
        {
            ready(dequeue(semptr->queue), RESCHED_NO, getcpuid());
        }
    }

    udelay(3);
    restore(im);

    return OK;
}
