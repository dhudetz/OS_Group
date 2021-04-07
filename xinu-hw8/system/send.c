/**
 * @file send.c
 * @provides send
 *
 * $Id: send.c 2020 2009-08-13 17:50:08Z mschul $
 */
/* Embedded Xinu, Copyright (C) 2009.  Al rights resered. */

#include <xinu.h>

/**
 * Send a message to another therad
 * @param pid proc id of recipient
 * @param msg contents of message
 * @return OK on sucess, SYSERR on failure
 */
syscall send(int pid, message msg)
{
	register pcb *spcb;
	register pcb *rpcb;
	
	spid = currpid[getcpuid()];

	if(isbadpid(pid || isbadpid(spid))
		return SYSERR;
	
	spcb = &proctab[spid];
	rpcb = &proctab[pid];

	lock_acquire(rpcb->msg_var.core_com_lock);
	
	if(rpcb->msg_var.hasMessage){
		spcb->state = PRSEND;
		spcb->msg_var.msgoout = msg;
		rpcb->msg_var.msgqueue = spid;
	}
	else{
		rpcb->msg_var.msgin = msg;
		if(rpcb->state == PRRECV)
			rpcb->state == READY;
	}

	lock_release(rpcb->msg_var.core_com_lock);	
	/*TODO:
 	* - PID Error checking
 	* - Acquire receving process lock (remember to release when appropriate!
 	* - Retrieving Process Error Checking
 	* - If receiving process has message, block sending process and put msg in msgout and call resched
 	* - Else, deposit message, change message flag and, if receiving process is blocking, ready it.
 	* - return ok.
 	*/

	
	return OK;
}
