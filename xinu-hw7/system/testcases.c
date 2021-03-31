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
        kprintf("FREE%dSIZE%d  ", (ulong)curr, curr->length);
	curr = curr->next;	
    }
    kprintf("\r\n\r\n");
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    uchar character;

    kprintf("===TEST BEGIN===\r\n");
    kprintf("0) Test priority scheduling\r\n");

    // TODO: Test your operating system!
    register memblk *a, *b, *c;
    character = kgetc();
    switch (character)
    {
    case '0':
	print_freelist(0);
        a = getmem(16);
	print_freelist(0);
	freemem(a, 1);
	print_freelist(0);
        break;
    case '1':
	print_freelist(0);
	a = getmem(16);
	print_freelist(0);
	freemem(a, 16);
	print_freelist(0);
	b = getmem(8);
	print_freelist(0);
	freemem(b, 8);
	print_freelist(0);
	break;
    case '2':
	print_freelist(0);
	a = getmem(16);
	print_freelist(0);
	freemem(a, 16);
	print_freelist(0);
	b = getmem(32);
	print_freelist(0);
	freemem(b, 32);
	print_freelist(0);
	break;
    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
