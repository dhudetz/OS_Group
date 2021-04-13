/**
 * COSC 3250 - Project 6
 * reschedule processes
 * @authors Danny Hudetz Marty Boehm
 * Instructor Rubya
 * TA-BOT:MAILTO daniel.hudetz@marquette.edu martin.boehm@marquette.edu
 */
#include <xinu.h>

extern void ctxsw(void *, void *);
/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */
syscall resched(void)
{
    int highest_prio;
    irqmask im;
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */

    uint cpuid = getcpuid();

    oldproc = &proctab[currpid[cpuid]];

    im = disable();

#if AGING

    // TODO: Implement aging.
    //
    //       Reference include/clock.h to find more information
    //       about the quantums and how aging should behave.
	int q = promote_medium[cpuid]--;
	if(q == 0){
		int deq_id = dequeue(readylist[cpuid][PRIORITY_MED]);
		enqueue(deq_id, readylist[cpuid][PRIORITY_HIGH]);
		promote_medium[cpuid] = QUANTUM;
	}
	q = promote_low[cpuid]--;
	if(q == 0){
		int deq_id = dequeue(readylist[cpuid][PRIORITY_LOW]);
		enqueue(deq_id, readylist[cpuid][PRIORITY_MED]);
		promote_low[cpuid] = QUANTUM;
	}

#endif

    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid[cpuid], readylist[cpuid][oldproc->priority]);
    }

    /* remove first process in highest priority ready queue */
    // determine queue to pick from
    if (nonempty(readylist[cpuid][PRIORITY_HIGH]))
    {
        highest_prio = PRIORITY_HIGH;
    }
    else if (nonempty(readylist[cpuid][PRIORITY_MED]))
    {
        highest_prio = PRIORITY_MED;
    }
    else
    {
        highest_prio = PRIORITY_LOW;
    }

    currpid[cpuid] = dequeue(readylist[cpuid][highest_prio]);
    newproc = &proctab[currpid[cpuid]];
    newproc->state = PRCURR;    /* mark it currently running    */

#if PREEMPT
    preempt[cpuid] = QUANTUM;
#endif

    ctxsw(&oldproc->regs, &newproc->regs);

    /* The OLD process returns here when resumed. */
    restore(im);
    return OK;
}
