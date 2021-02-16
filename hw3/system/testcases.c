/**
 * @file testcases.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 *
 * Modified by:
 *
 * and
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

devcall putc(int dev, char c)
{
    return 0;
}


/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
	int testNum = 0;
	int i = 0;
	char word[] = "hello";
	kprintf("===TEST BEGIN===\r\n");
	while(testNum != 'q')
	{
		kprintf("Select test number (1->10) or 'q' to quit.\r\n");
 		testNum = kgetc();
		switch (testNum)
		{
		case 'q':
			kprintf("Quitting...\r\n");
			break;
		case '1':	
			kprintf("Test 1: Type char 'a': \r\n");
			int c = kgetc();
			switch (c)
			{
			case 'a':
				kprintf("PASSED: Character correct.\r\n");
				break;
			default:
				kprintf("FAILED: Character '%c' detected instead of 'a'.\r\n", c);
				break;
			}
			break;
		case '2':
			kprintf("Test 2: Type \"hello\": \r\n");
			int flag = 1;
			for(i = 0; i<5; i++)
			{
				int c = kgetc();
				if(word[i] != c)
					flag = 0;
			}
			switch(flag)
			{
			case(0):
				kprintf("FAILED: String other than 'hello' detected.\r\n");
				break;
			case(1):
				kprintf("PASSED: String detected correctly.\r\n");
				break;
			}
			break;
		case '3':
			kprintf("Test 3: kputc() test.\r\n");
			kputc('*');
			kprintf("\r\nIf you see a '*' char above, then test PASSED. Else, FAILED.\r\n");
			break;
		case '4':
			kprintf("Test 4: Multiple kputc() test. \r\n");
			for(i = 0; i < 5; i++)
			{
				kputc(word[i]);
			}
			kprintf("\r\nIf you see 'hello' above, then test PASSED. Else, FAILED.\r\n");
			break;
    		case '5':
			kprintf("Test 5: kcheckc() no input test.\r\n");
			int check = kcheckc();
			switch(check)
			{
			case 0:
				kprintf("PASSED: No input from UART detected.\r\n");
				break;	
			case 1:
				kprintf("FAILED: Unexpected char detected.\r\n");
				break;
			}
			break;
		default:
			kprintf("Input valid test number.\r\n");
			break;
		}
	}

	kprintf("\r\n===TEST END===\r\n");
	return;
}
