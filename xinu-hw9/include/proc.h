/**
 * @file proc.h
 * @provides isbadpid
 *
 * COSC 3250 / COEN 4830 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2010.  All rights reserved. */

#ifndef _PROC_H_
#define _PROC_H_

#include <arm.h>
#include <semaphore.h>

/* process table declarations and defined constants                      */

#ifndef NPROC                   /*   if number of processes is not set,    */
#define NPROC       50        /**< set the number of processes           */
#endif

/* unusual value marks the top of the process stack                      */
#define STACKMAGIC 0x0A0AAA99

/* process state constants                                               */

#define PRFREE      0       /**< process slot is free                    */
#define PRCURR      1       /**< process is currently running            */
#define PRSUSP      2       /**< process is suspended                    */
#define PRREADY     3       /**< process is on ready queue               */
#define PRRECV      4       /**< process is blocked pending a send       */
#define PRSEND      5       /**< process is blocked pending a recv       */

/* miscellaneous process definitions                                     */

#define PNMLEN      16      /**< length of process "name"                */

/* Reschedule constants for ready  */
#define RESCHED_YES 1           /**< tell ready to reschedule           */
#define RESCHED_NO  0           /**< tell ready not to reschedule       */

/* the null process is always eligible to run                            */
#define NULLPROC    0       /**< id of the null process                  */
#define NULLPROC1   1
#define NULLPROC2   2
#define NULLPROC3   3
#define BADPID     (-1)     /**< used when invalid pid needed            */

/** 
 * Check for invalid process ids.  Note that interrupts must be disabled
 * for the condition to hold true between statements.
 */
#define isbadpid(x) ((x)<0 || (x)>=NPROC || PRFREE == proctab[(x)].state)

/* process message passing structure */
typedef struct pcb_msg
{
        int hasMessage;
        message msgin;
        message msgout;
        qid_typ msgqueue;
        spinlock_t core_com_lock;

} pmessage;
/* process table entry */

typedef struct pentry
{
	int state;                  /**< process state: PRCURR, etc.             */
	int priority;				/**< priority of process                     */
	void *stkbase;              /**< base of run time stack                  */
	int stklen;                 /**< stack length                            */
	int core_affinity;          /**< core affinity                           */
	char name[PNMLEN];          /**< process name                            */
	int regs[PREGS];			/**< stored process registers                */
	struct pcb_msg msg_var;
	semaphore sem;
} pcb;

/* process initialization constants */
#define INITSTK  65536      /**< initial process stack size              */
#define INITPRIO 1          /**< initial process priority                */
#define INITRET  userret    /**< processes return address                */
#define MINSTK   4096       /**< minimum process stack size              */
#define NULLSTK  MINSTK     /**< null process stack size                 */

/* Priority constants */
#define PRIORITY_LOW	0	/**< low process priority					 */
#define PRIORITY_MED	1	/**< medium process priority				 */
#define PRIORITY_HIGH	2	/**< high process priority					 */

extern struct pentry proctab[];
extern int numproc;         /**< currently active processes              */
extern int currpid[];         /**< currently executing process             */

#endif                          /* _PROC_H_ */
