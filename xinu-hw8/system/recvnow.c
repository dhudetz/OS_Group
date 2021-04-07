/**
 * @file recvnow.c
 * @provides recvnow
 *
 */
/* Embedded Xinu, Copyright (C) 2020.  All rights resered. */

#include <xinu.h>

/**
 * recvnow - return pre-stored message, or immediate error.
 * @return message or SYSERR
 */

message recvnow(void)
{
	register pcb *ppcb;
	message msg;

	ppcb = &proctab[currpid[getcpuid()]];
	
	lock_acquire(ppcb->msg_var.core_com_lock);
	if(ppcb->msg_var.hasMessage){
		msg = ppcb->msg_var.msgin;
		lock_release(ppcb->msg_var.core_com_lock);
		return msg;
	}
	else{
		lock_release(ppcb->msg_var.core_com_lock);
		return SYSERR;
	}	
	
	/* TODO:
 	* - Acquire and relsase lock when working in msg structure
 	* - check for message, if no messsage, error
 	*   		       else, retrieve & return message
 	*/ 
}
