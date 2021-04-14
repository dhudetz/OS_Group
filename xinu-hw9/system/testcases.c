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

/* Function prototypes */
void print_sem(semaphore sem);
uchar getc(void);
void putc(uchar);


/* Test process to use for testing semaphores. */
process testSemWait(semaphore sem)
{
    wait(sem);
    printf("process %d after wait()\r\n", currpid[getcpuid()]);
    return 0;
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    uchar c;
    semaphore testsem;

    enable();
    char word[] = "hello";
    int i = 0;
    printf("===TEST BEGIN===\r\n");
    printf("Tests 1->8\r\n");
	// TODO: Test your operating system!
    c = getc();
    switch (c)
    {
    case '1':
	printf("Test 1: Type char 'a': \r\n");
	c = getc();
	switch (c)
	{
	case 'a':
		printf("PASSED: Character correct.\r\n");
		break;
	default:
		printf("FAILED: Character '%c' detected instead of 'a'.\r\n", c);
		break;
	}
	break;
    case '2':
	printf("Test 2: Type \"hello\": \r\n");
	int flag = 1;
	for(i = 0; i<5; i++)
	{
		int c = getc();
		if(word[i] != c)
			flag = 0;
	}
	switch(flag)
	{
	case(0):
		printf("FAILED: String other than 'hello' detected.\r\n");
		break;
	case(1):
		printf("PASSED: String detected correctly.\r\n");
		break;
	}
	break;
    case '3':
	printf("Test 3: putc() test.\r\n");
	putc('*');
	printf("\r\nIf you see a '*' char above, then test PASSED. Else, FAILED.\r\n");
	break;
    case '4':
	printf("Test 4: Multiple putc() test. \r\n");
	for(i = 0; i < 5; i++)
	{
		putc(word[i]);
	}
	printf("\r\nIf you see 'hello' above, then test PASSED. Else, FAILED.\r\n");
	break;
    case '5':
	printf("Test 5: Semaphore wait test. \r\n");
	testsem = semcreate(1);
	print_sem(testsem);
	wait(testsem);
	print_sem(testsem);
	break;
    case '6':
	printf("Test 5: Semaphore signal test. \r\n");
	testsem = semcreate(1);
	print_sem(testsem);
	signal(testsem);
	print_sem(testsem);
	break;
    case '7':
	printf("Test 7: Semaphore wait signal wait test. \r\n");
	testsem = semcreate(1);
	print_sem(testsem);
	wait(testsem);
	print_sem(testsem);
	signal(testsem);
	print_sem(testsem);
	wait(testsem);
	print_sem(testsem);

	break;
    case '8':
	printf("Test 8: Semaphore signaln test. \r\n");
	testsem = semcreate(1);
	print_sem(testsem);
	signaln(testsem, 3);
	print_sem(testsem);
	break;
    default:
        break;
    }

    while (numproc > 5)  // Four null procs plus this test proc.
    {
        resched();
    }

    printf("\r\n===TEST END===\r\n");
    printf("\r\n\r\nAll user processes have completed.\r\n\r\n");

    while (1);
    return;
}

void print_sem(semaphore sem)
{
    struct sement *semptr;
    semptr = &semtab[sem];

    printf("\r\n%d->state: %s\r\n", sem,
           (semptr->state == 0x01) ? "SFREE" : "SUSED");
    printf("%d->count: %d\r\n", sem, semptr->count);
    printf("%d->queue: %s\r\n\r\n", sem,
           (isempty(semptr->queue)) ? "EMPTY" : "NONEMPTY");
}
