/**
 *COSC 3250 - Project 3
 *Testcases for kprintf function
 *@author Martin Boehm and Danny Hudetz
 *Instructor Sabirat Rubya
 *TA-BOT:MAILTO martin.boehm@marquette.edu daniel.hudetz@marquette.edu
 */

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
	int check = 0;
	kprintf("===TEST BEGIN===\r\n");
	while(testNum != 'q')
	{
		kprintf("Select test number (1->9) or 'q' to quit.\r\n");
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
			check = kcheckc();
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
		case '6':
			kprintf("Test 6: kcheckc() buffer test.\r\n");
			kungetc('c');
			check = kcheckc();
			switch(check)
			{
			case 0:
				kprintf("FAILED: Char in buffer undetected.\r\n");
				break;
			case 1:
				kprintf("PASSED: Char in buffer detected.\r\n");
			kgetc();
			break;
			}
		case '7':
			kungetc('a');
			c = kgetc();
			switch(c)
			{
			case 'a':
				kprintf("PASSED: Character 'a' was recieved from unget buffer.\r\n");
				break;
			default:
				kprintf("FAILED: Character 'a' was not recieved from unget buffer.\r\n");
				break;
			}
			break;
		case '8':
			kungetc('a');
			kungetc('b');
			kungetc('c');
			kgetc();
			c = kgetc();
			switch(c)
			{
			case 'b':
				kprintf("PASSED: Character 'b' was recieved on the kgetc call.\r\n");
				break;
			default:
				kprintf("FAILED: Character 'b' was not recieved on the kgetc call.\r\n");
				break;
			}
			kgetc();
			break;
		case '9':
			for(i = 0; i < 10; i++)
			{
				kungetc('a');
			}
			c = kungetc('a');
			switch(c)
			{
			case -1:
				kprintf("PASSED: SYSERR was thrown, too many entries in unget buffer.\r\n");
				break;
			default:
				kprintf("FAILED: SYSERR was not thrown, even though unget buffer was overfilled.\r\n");
				break;
			}
			for(i = 0; i < 10; i++)
			{
				kgetc();
			}
			break;
		default:
			kprintf("Input valid test number.\r\n");
			break;
		}
		kprintf("\r\n");
	}

	kprintf("\r\n===TEST END===\r\n");
	return;
}
