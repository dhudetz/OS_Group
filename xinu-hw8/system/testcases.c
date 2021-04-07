/**
 * COSC 3250 - Project 8
 * Test cases for process messages
 * @authors Danny Hudetz Marty Boehm
 * Instructor Rubya
 * TA-BOT:MAILTO daniel.hudetz@marquette.edu martin.boehm@marquette.edu
 */

#include <xinu.h>

/**
 * testcases - called after initialization completes to test things.
 */

void receiveMsg(void)
{
    	register pcb *currpcb;

	currpcb = &proctab[currpid[getcpuid()]];
	while(1==1){
		if(currpcb->msg_var.hasMessage){
			kprintf("Message received: %d/r/n", recv());
		}			
	}
}

void receiveMsgNow(void)
{
    	register pcb *currpcb;

	currpcb = &proctab[currpid[getcpuid()]];
	while(1==1){
		if(currpcb->msg_var.hasMessage){
			kprintf("Message received: %d/r/n", recvnow());
		}			
	}
}

void testcases(void)
{
    uchar c;

    kprintf("===TEST BEGIN===\r\n");
    kprintf("0) One SENDNOW()\r\n");
    kprintf("1) One sendnow and RECV\r\n");
    kprintf("2) sendnow and SEND enqueue test\r\n");
    kprintf("3) One sendnow and one RECVNOW\r\n");

    // TODO: Test your operating system!

    c = kgetc();

    int result;
    register pcb *ppcb;
    register pcb *currpcb;
    int testpid;
    int currid;
    switch (c)
    {
    case '0':
	testpid = create((void *)receiveMsg, INITSTK, PRIORITY_LOW, "RECV", 0);
	ppcb = &proctab[testpid];
	ppcb->core_affinity = 0;
	result = sendnow(testpid, 0x5);
	kprintf("Result: %d", result);
	if((TRUE == ppcb->msg_var.hasMessage) && (0x5 == ppcb->msg_var.msgin))
		kprintf("Message correctly sent.\r\n");
	else
		kprintf("Recv process hasMessage == %d, msgin == %d\r\n", ppcb->msg_var.hasMessage, ppcb->msg_var.msgin);
        kill(testpid);
	break;
    case '1':
	testpid = create((void *)receiveMsg, INITSTK, PRIORITY_LOW, "RECV", 0);
	ppcb = &proctab[testpid];
	ppcb->core_affinity = 0;
	currid = currpid[getcpuid()];
	currpcb = &proctab[currid];
	result = sendnow(testpid, 0x5);
	kprintf("Result: %d\n\r", result);
	ready(testpid, RESCHED_NO, 0);
	if((TRUE == ppcb->msg_var.hasMessage) && (0x5 == ppcb->msg_var.msgin))
		kprintf("Messages correctly sent.\r\n");
	else
		kprintf("Recv process hasMessage == %d, msgin == %d\r\n", ppcb->msg_var.hasMessage, ppcb->msg_var.msgin);
        kill(testpid);
	break;

    case '2':
	testpid = create((void *)receiveMsg, INITSTK, PRIORITY_LOW, "RECV", 0);
	ppcb = &proctab[testpid];
	ppcb->core_affinity = 0;
	currid = currpid[getcpuid()];
	currpcb = &proctab[currid];
	result = sendnow(testpid, 0x5);
	kprintf("Result: %d\n\r", result);
	result = send(testpid, 0x6);
	kprintf("Result: %d\n\r", result);
	if((TRUE == ppcb->msg_var.hasMessage) && (0x5 == ppcb->msg_var.msgin) && 
		(currid == ppcb->msg_var.msgqueue) && (currpcb->msg_var.msgout == 0x6))
		kprintf("Messages correctly sent.\r\n");
	else
		kprintf("Recv process hasMessage == %d, msgin == %d\r\n", ppcb->msg_var.hasMessage, ppcb->msg_var.msgin);
        kill(testpid);
	break;

    case '3':
	testpid = create((void *)receiveMsgNow, INITSTK, PRIORITY_LOW, "RECV", 0);
	ppcb = &proctab[testpid];
	ppcb->core_affinity = 0;
	currid = currpid[getcpuid()];
	currpcb = &proctab[currid];
	result = sendnow(testpid, 0x5);
	kprintf("Result: %d\n\r", result);
	if((TRUE == ppcb->msg_var.hasMessage) && (0x5 == ppcb->msg_var.msgin))
		kprintf("Messages correctly sent.\r\n");
	else
		kprintf("Recv process hasMessage == %d, msgin == %d\r\n", ppcb->msg_var.hasMessage, ppcb->msg_var.msgin);
        kill(testpid);
	break;

    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
