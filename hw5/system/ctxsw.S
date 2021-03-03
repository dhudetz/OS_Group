/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */

#include <arm.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldregs, &newregs)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldstack address of outgoing stack save area
 * @param  &newstack address of incoming stack save area
 * @return special case -- see above
 */
ctxsw:
	.func ctxsw

	// TODO: Save callee-save ("non-volatile") registers.
	// WORKED ON LOGIC WITH BOB DIRMISH
	str	PREG_R1,[PREG_R0, 4]
	str	PREG_R2,[PREG_R0, 8]
	str	PREG_R3,[PREG_R0, 12]
	str	R4,[R0, #PREG_R4 * 4]
	str	R5,[R0, #PREG_R4 * 5]
	str	R6,[R0, #PREG_R4 * 6]
	str	R7,[R0, #PREG_R4 * 7]
	str	R8,[R0, #PREG_R4 * 8]
	str	R9,[R0, #PREG_R4 * 9]
	str	R10,[R0, #PREG_R4 * 10]
	str	R11,[R0, #PREG_R4 * 11]
	str	R12,[R0, #PREG_R4 * 12]
	str	R13,[R0, #PREG_R4 * 13]
	str	R14,[R0, #PREG_R4 * 14]
	str	R15,[R0, #PREG_R4 * 15]

	mov	R12, R0

	// TODO: Restore callee-save ("non-volatile") registers.
	// WORKED ON LOGIC WITH BOB DIRMISH
	mov	R1, R12

	ldr	PREG_R0,[PREG_R1, 0]
	ldr	PREG_R1,[PREG_R1, 4]
	ldr	PREG_R2,[PREG_R1, 8]
	ldr	PREG_R3,[PREG_R1, 12]
	ldr	R4,[R1, #PREG_R4 * 4]
	ldr	R5,[R1, #PREG_R4 * 5]
	ldr	R6,[R1, #PREG_R4 * 6]
	ldr	R7,[R1, #PREG_R4 * 7]
	ldr	R8,[R1, #PREG_R4 * 8]
	ldr	R9,[R1, #PREG_R4 * 9]
	ldr	R10,[R1, #PREG_R4 * 10]
	ldr	R11,[R1, #PREG_R4 * 11]
	ldr	R12,[R1, #PREG_R4 * 12]
	ldr	R13,[R1, #PREG_R4 * 13]
	ldr	R14,[R1, #PREG_R4 * 14]
	ldr	R15,[R1, #PREG_R4 * 15]

	// TODO: Jump to next function.

	.end ctxsw
