/**
 * COSC 3250 - Assignment 10
 * Allows xinu to delete files, at least I really hope so
 * @authors Marty Boehm Danny Hudetz
 * Instructor Sabirat Rubya
 * TA-BOT:MAILTO martin.boehm@marquette.edu
 */


/* sbFreeBlock.c - sbFreeBlock */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*                                                                 */
/* and                                                             */
/*                                                                 */
/*                                                                 */

#include <kernel.h>
#include <device.h>
#include <memory.h>
#include <disk.h>
#include <file.h>

/*------------------------------------------------------------------------
 * sbFreeBlock - Add a block back into the free list of disk blocks.
 *------------------------------------------------------------------------
 */
devcall sbFreeBlock(struct superblock *psuper, int block)
{
	//declare the variables
	struct dentry *phw;
	struct freeblock *freeblk, *free2;
	struct dirblock *swizzle;
	int diskfd;

	//error check and calc the diskfd
	if(NULL == psuper)
	{
		return SYSERR;
	}
	phw = psuper->sb_disk;
	if(NULL == phw)
	{
		return SYSERR;
	}
	diskfd = phw - devtab;
	freeblk = psuper->sb_freelst;
	if(NULL == freeblk)
	{
		return SYSERR;
	}
	
	//set up the location of where to put the block
	while(freeblk->fr_next != NULL)
	{
		freeblk = freeblk->fr_next;
	}
	
	//grab the lock
	wait(psuper->sb_freelock);

	//actually place the block back into the list of freeblocks (remeber to check for every case!)
	//seems to be 3 cases: if there is no freeblock, if there is one and it is full, or if there is one and it isn't full
	//start with if there isn't one
	if(!freeblk)
	{
		freeblk = (struct freeblock*)malloc(sizeof(struct freeblock*));
		if(NULL == freeblk)
		{
			signal(psuper->sb_freelock);
			return SYSERR;
		}
		psuper->sb_freelst = NULL;
		swizzle = psuper->sb_dirlst;
		psuper->sb_dirlst = (struct dirblock*)swizzle->db_blocknum;
		seek(diskfd, psuper->sb_blocknum);
		if(SYSERR == write(diskfd, psuper, sizeof(struct superblock)))
		{
			return SYSERR;
		}
		psuper->sb_dirlst = swizzle;
		freeblk->fr_blocknum = (int)freeblk;
		freeblk->fr_next = 0;
		freeblk->fr_count = 1;
		freeblk->fr_free[freeblk->fr_count - 1] = block;
		seek(diskfd, freeblk->fr_blocknum);
		if(SYSERR == write(diskfd, freeblk, sizeof(struct freeblock)))
		{
			return SYSERR;
		}
		signal(psuper->sb_freelock);
	}
	
	//now, go with if there is one but it is full
	if(freeblk->fr_count >= FREEBLOCKMAX)
	{
		free2 = (struct freeblock*)malloc(sizeof(struct freeblock*));
		free2->fr_blocknum = (int)free2;
		free2->fr_next = 0;
		free2->fr_count = 1;
		free2->fr_free[free2->fr_count - 1] = block;
		freeblk->fr_next = free2;
		seek(diskfd, free2->fr_blocknum);
		if(SYSERR == write(diskfd, free2, sizeof(struct freeblock)))
		{
			return SYSERR;
		}
		signal(psuper->sb_freelock);
	}

	//lastly, the block is present AND it is not full
	if(NULL == freeblk->fr_next)
	{
		freeblk->fr_count++;
		freeblk->fr_free[freeblk->fr_count - 1] = block;
		seek(diskfd, freeblk->fr_blocknum);
		if(SYSERR == write(diskfd, freeblk, sizeof(struct freeblock)))
		{
			return SYSERR;
		}
	}

	//release the lock
	signal(psuper->sb_freelock);

	// TODO: Add the block back into the filesystem's list of
 	//  free blocks.  Use the superblock's locks to guarantee
	//  mutually exclusive access to the free list, and write
	//  the changed free list segment(s) back to disk.
	return SYSERR;
}
