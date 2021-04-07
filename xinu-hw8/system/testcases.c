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
	while(1){
		kprintf("");			
	}
}

void testcases(void)
{
    uchar c;

    kprintf("===TEST BEGIN===\r\n");
    kprintf("0) Test priority scheduling\r\n");
    kprintf("\'A\') Aging / Starvation testcase\r\n");
    kprintf("\'P\') Preemption testcase\r\n");

    // TODO: Test your operating system!

    c = kgetc();

    int result;
    register pcb *ppcb;
    int testpid;
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

    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
