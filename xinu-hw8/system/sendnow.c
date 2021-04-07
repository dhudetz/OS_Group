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
	register pcb *spcb;
	register pcb *rpcb;
	
	int ppid = currpid[getcpuid()];

	if(isbadpid(ppid) || isbadpid(pid))
		return SYSERR;

	spcb = &proctab[ppid];
	rpcb = &proctab[pid];

	lock_acquire(rpcb->msg_var.core_com_lock);

	if(rpcb->msg_var.hasMessage){
		lock_release(rpcb->msg_var.core_com_lock);
		return SYSERR;
	}
	else{
		rpcb->msg_var.msgin = msg;
		rpcb->msg_var.hasMessage = TRUE;
		lock_release(rpcb->msg_var.core_com_lock);
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
