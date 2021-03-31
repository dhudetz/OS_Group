/**
 * @file testcases.c
 * @provides testcases
 *
 *
 * Modified by:	
 *
 * TA-BOT:MAILTO 
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

void print_freelist(int corenum)
{
    register memblk *curr;
    curr = freelist[corenum].head;
    while(curr){
        kprintf("FREE%dSIZE%d ", (ulong)curr, curr->length);
	curr = curr->next;	
    }
    kprintf("\n");
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    uchar c;

    kprintf("===TEST BEGIN===\r\n");
    kprintf("0) Test priority scheduling\r\n");

    // TODO: Test your operating system!

    c = kgetc();
    switch (c)
    {
    case '0':
	print_freelist(0);
        register memblk *a = getmem(16);
	print_freelist(0);
	freemem(a, 1);
	print_freelist(0);
        break;
    case '1':
	print_freelist(0);
	register memblk *b = getmem(16);
	print_freelist(0);
	freemem(b, 16);
	print_freelist(0);
	register memblk *c = getmem(8);
	print_freelist(0);
	freemem(c, 8);
	print_freelist(0);
	break;

    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
