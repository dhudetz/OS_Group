/**
 * @file recv.c
 * @provides recv.
 *
 * $Id: receive.c 2020 2009-08-13 17:50:08Z mschul $
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * recv - wait for a mesage and return it
 * @return message
 */

message recv(void)
{
	register pcb *ppcb;
	register pcb *spcb;
	int senderpid = -1;
	message msg;
	ppcb = &proctab[currpid[getcpuid()]];
	
	lock_acquire(ppcb->msg_var.core_com_lock);

	if(ppcb->msg_var.hasMessage)
	{
		msg = ppcb->msg_var.msgin;
		ppcb->msg_var.hasMessage = FALSE;
		ppcb->state = PRREADY;
	}
	else{
		ppcb->state = PRRECV;
		//resched();
	}
	if(ppcb->msg_var.msgqueue != EMPTY)
	{
		spcb = &proctab[ppcb->msg_var.msgqueue];
		ppcb->msg_var.msgin = spcb->msg_var.msgout;
		ppcb->msg_var.hasMessage = TRUE;
		spcb->state = PRREADY;
		ppcb->msg_var.msgqueue = EMPTY;
	}
	else{
		ppcb->msg_var.hasMessage = FALSE;	
	}

	/* TODO:
 	* - Remember to acquire lock and release lock when interacitng with the msg structure
 	* - Check for Message. If no message, put in blocking state and reschedule
 	*   			If there is message, retrive message
 	* - Now, check queue for wating send processes,
 	*   If stuff in queue  collect message for next time recv is called and ready send process
 	*   else, reset message flag
 	*   return collected message
 	*/	
	
	lock_release(ppcb->msg_var.core_com_lock);
	return msg;
}
