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
	int senderpid = -1;
	message msg;
	ppcb = &proctab[currpid[getcpuid()]];
	
	lock_acquire(ppcb->msg_var.core_com_lock);

	if(ppcb->msg_var.hasMessage)
	{
		msg = ppcb->msg_var.msgin;
	}

	else if(nonempty(ppcb->msg_var.msgqueue))
	{
		msg = ppcb->msg_var.msgout;
		ppcb->msg_var.msgin = msg;
		ppcb->state = PRREADY;
	}
	
	else if(isempty(ppcb->msg_var.msgqueue))
	{
		ppcb->msg_var.hasMessage = 0;
	}
	
	else
	{
		ppcb->state = PRSEND;
		lock_release(ppcb->msg_var.core_com_lock);
		resched();
		lock_acquire(ppcb->msg_var.core_com_lock);
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
	

	return msg;
}
