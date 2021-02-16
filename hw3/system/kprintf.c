/**
 * @file kprintf.c
 */

/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

#define UNGETMAX 10             /* Can un-get at most 10 characters. */

static unsigned char ungetArray[UNGETMAX] = {NULL};

/**
 * Synchronously read a character from a UART.  This blocks until a character is
 * available.  The interrupt handler is not used.
 *
 * @return
 *      The character read from the UART as an <code>unsigned char</code> cast
 *      to an <code>int</code>.
 */
syscall kgetc(void)
{
	volatile struct pl011_uart_csreg *regptr;
	
 	/* Pointer to the UART control and status registers.  */
 	regptr = (struct pl011_uart_csreg *)0x3F201000;
	if(ungetArray[0] != NULL)
	{
		int i;
		for(i = UNGETMAX - 1; i>=0; i--)
		{
			if(ungetArray[i] != NULL)
			{
				unsigned char buffer = ungetArray[i];
				ungetArray[i] = NULL;
				return buffer;
			}
		}
	}
	else
	{
		while((regptr->fr & PL011_FR_RXFE) != 0)
			continue;
		return (int)regptr->dr;
	}
	
	return SYSERR;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
	volatile struct pl011_uart_csreg *regptr;
 	regptr = (struct pl011_uart_csreg *)0x3F201000;
	if(ungetArray[0] != NULL || (regptr->fr & PL011_FR_RXFE) == 0)
		return 1;
	return 0;
}

/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
	// TODO: Check for room in unget buffer, put the character in or discard.
	int i;
	for(i = 0; i<UNGETMAX; i++)
	{
		if(ungetArray[i] == NULL)
		{
			ungetArray[i] = c;
			return (int) c;
		}
	}
	return SYSERR;
}


/**
 * Synchronously write a character to a UART.  This blocks until the character
 * has been written to the hardware.  The interrupt handler is not used.
 *
 * @param c
 *      The character to write.
 *
 * @return
 *      The character written to the UART as an <code>unsigned char</code> cast
 *      to an <code>int</code>.
 */
syscall kputc(uchar c)
{
	volatile struct pl011_uart_csreg *regptr;
	
	/* Pointer to the UART control and status registers.  */
	regptr = (struct pl011_uart_csreg *)0x3F201000;
	int isEmpty = 0;
	while(isEmpty==0)
		isEmpty = (regptr->fr & PL011_FR_TXFE);
	regptr->dr = c;
	return (int) c;

	return SYSERR;
}

/**
 * kernel printf: formatted, synchronous output to SERIAL0.
 *
 * @param format
 *      The format string.  Not all standard format specifiers are supported by
 *      this implementation.  See _doprnt() for a description of supported
 *      conversion specifications.
 * @param ...
 *      Arguments matching those in the format string.
 *
 * @return
 *      The number of characters written.
 */
syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(int, int))kputc, 0);
    va_end(ap);
    return retval;
}
