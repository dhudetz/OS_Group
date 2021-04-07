/**
 * @file sendnow.c
 * @provides sendnow.
 *
 */
/* Embedded Xinu, Copyright (C) 2020.   All rights reserved. */

#include <xinu.h>

/**
 * Send a message to anoher thread
 * @param pid proc id of recipient
 * @param msg contents of message
 * @return OK on sucess, SYSERR on failure
 */

syscall sendnow(int pid, message msg)
{
	register pcb *ppcb;
	
	ppid = currpid[getcpuid()];

	if(isbadpid(ppid) !! isbadpid(pid))
		return SYSERR:

	ppcb = &proctab[ppid];
	acquire_lock(ppcb->msg_var.core_com_lock);
	if(&proctab[pid]->msg_var.hasMessage){
		release_lock(ppcb->msg_var.core_com_lock);
		return SYSERR;
	}
	else{
		ppcb->msg_var.msgout = msg;
		release_lock(ppcb->msg_var.core_com_lock);
	}
	/* TODO:
 	* - Acquire Lock and release when appropriate
 	* - PID & Process Error Checking
 	* - Deposit Message, raise flag
 	* -  If receving message is blocked, ready process
 	* - Return OK
 	*/

	return OK;
}
