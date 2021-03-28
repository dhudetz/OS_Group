/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <arm.h>
#include <xinu.h>

/* Assembly routine for atomic operations */
extern int _atomic_increment(int *);
extern int _atomic_increment_mod(int *, int);

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, ulong priority, char *name, ulong nargs, ...)
{
	ulong *saddr;               /* stack address                */
	ulong pid;                  /* stores new process id        */
	pcb *ppcb;                  /* pointer to proc control blk  */
	ulong i;
	va_list ap;                 /* points to list of var args   */
	ulong pads = 0;             /* padding entries in record.   */
	void INITRET(void);

	if (ssize < MINSTK)
		ssize = MINSTK;
	ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;
	/* round up to even boundary    */
	saddr = (ulong *)getstk(ssize);     /* allocate new stack and pid   */
	pid = newpid();
	/* a little error checking      */
	if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
	{
		return SYSERR;
	}

	_atomic_increment(&numproc);

	ppcb = &proctab[pid];
	/* setup PCB entry for new proc */
	ppcb->state = PRSUSP;
	ppcb->stkbase = saddr;
	ppcb->stklen = ssize;
	ppcb->core_affinity = -1;
	ppcb->priority = priority;
	strncpy(ppcb->name, name, sizeof(name));

	// TODO: Setup PCB entry for new process.


	/* Initialize stack with accounting block. */
	*saddr = STACKMAGIC;
	*--saddr = pid;
	*--saddr = ppcb->stklen;
	*--saddr = (ulong)ppcb->stkbase;

	/* Handle variable number of arguments passed to starting function   */
	if (nargs)
	{
		pads = ((nargs - 1) / 4) * 4;
	}
	/* If more than 4 args, pad record size to multiple of native memory */
	/*  transfer size.  Reserve space for extra args                     */
	for (i = 0; i < pads; i++)
	{
		*--saddr = 0;
	}
	//Access unnamed variables
	int p;
	int counter = 0;
	va_start(ap, nargs);
	while((p = (va_arg(ap, int))))
	{
		if(counter<4)
		{
			ppcb->regs[counter] = p;
		}
		else
		{
			*(saddr+counter-4) = p;
		}
		counter++;
	}
	va_end(ap);
	ppcb->regs[PREG_SP] = (int)saddr;
	ppcb->regs[PREG_LR] = (int)userret;
	ppcb->regs[PREG_PC] = (int)funcaddr;

	//Fill registers 0-3 with first arguments
	
	//If there is more arguments fill stack and pass pointer

	// TODO: Initialize process context.
	//
	// TODO:  Place arguments into activation record.
	//        See K&R 7.3 for example using va_start, va_arg and
	//        va_end macros for variable argument functions.

	return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
	pid_typ pid;                /* process id to return     */
	static pid_typ nextpid = 0;

	for (pid = 0; pid < NPROC; pid++)
	{                           /* check all NPROC slots    */
		//        nextpid = (nextpid + 1) % NPROC;
		_atomic_increment_mod(&nextpid, NPROC);
		if (PRFREE == proctab[nextpid].state)
		{
			return nextpid;
		}
	}
	return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
	uint cpuid = getcpuid();
	kill(currpid[cpuid]);
}
