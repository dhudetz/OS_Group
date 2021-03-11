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
	str	R4,[R0, #PREG_R4 * 4]
	str	R5,[R0, #PREG_R5 * 4]
	str	R6,[R0, #PREG_R6 * 4]
	str	R7,[R0, #PREG_R7 * 4]
	str	R8,[R0, #PREG_R8 * 4]
	str	R9,[R0, #PREG_R9 * 4]
	str	R10,[R0, #PREG_R10 * 4]
	str	R11,[R0, #PREG_R11 * 4]
	str	R12,[R0, #PREG_IP * 4]
	str	R13,[R0, #PREG_SP * 4]
	str	R14,[R0, #PREG_LR * 4]
	str	R14,[R0, #PREG_PC * 4]	

	// TODO: Restore callee-save ("non-volatile") registers.
	
	ldr	R4,[R1, #PREG_R4 * 4]
	ldr	R5,[R1, #PREG_R5 * 4]
	ldr	R6,[R1, #PREG_R6 * 4]
	ldr	R7,[R1, #PREG_R7 * 4]
	ldr	R8,[R1, #PREG_R8 * 4]
	ldr	R9,[R1, #PREG_R9 * 4]
	ldr	R10,[R1, #PREG_R10 * 4]
	ldr	R11,[R1, #PREG_R11 * 4]
	ldr	R12,[R1, #PREG_IP * 4]
	ldr	R13,[R1, #PREG_SP * 4]
	
	// link to where old program left off
	ldr	R14,[R1, #PREG_LR * 4]
	
	// next instrucion at beginning of new program
	ldr	R15,[R1, #PREG_PC * 4]

	// TODO: Jump to next function.

	.end ctxsw
