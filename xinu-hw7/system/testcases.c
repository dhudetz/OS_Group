
/**
* COSC 3250 - Project 7
* Explain briefly the functionality of the program.
* * @authors Danny Hudetz Martin Boehm
* * Instructor Sabirat Rubya
* * TA-BOT:MAILTO daniel.hudetz@marquette.edu martin.boehm@marquette.edu
* */


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
    kprintf("0: Basic getmem and freemem test\r\n");
    kprintf("1: Call getmem, free it, call it again, and free it\r\n");
    kprintf("2: Call getmem, free it, call for a mem larger than the splintered portion, and free\r\n");
    kprintf("3: Call getmem twice, free it twice\r\n");
    kprintf("4: Call getmem of 0\r\n");
    kprintf("5: Check to see what happens when getmem is called with a requested 1 bit\r\n");

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
    case '3':
	print_freelist(0);
	a = getmem(64);
	print_freelist(0);
	b = getmem(32);
	print_freelist(0);
	freemem(a, 64);
	print_freelist(0);
	freemem(b, 32);
	print_freelist(0);
	break;
    case '4':
	if(getmem(0) == SYSERR)
	{
	    kprintf("SYSERR HAS OCCURED (THIS IS A GOOD THING)\r\n");
	}
	break;
    case '5':
	print_freelist(0);
	a = getmem(1);
	print_freelist(0);
	freemem(a, 1);
	print_freelist(0);
	break;
    case '6':
	print_freelist(0);
	a = malloc(8);
	print_freelist(0);
	free(a);
	print_freelist(0);
	break;
    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
