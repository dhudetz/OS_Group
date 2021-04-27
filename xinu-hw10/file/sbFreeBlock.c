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
	struct freeblock *freeblk;
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
	freeblk = psuper->sb_freelist;
	if(NULL == freeblk)
	{
		printf("sbGetFree() ERROR: SUPERBLOCK FREELIST IS EMPTY!\n");
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
		psuper->sb_freelist = NULL;
		swizzle = psuper->sb_dirlst;
		psuper-sb_dirlst = (struct dirblock *)swizzle->db_blocknum;
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
		seek(DISK0, spuertab->sb_dirlst->db_blocknum);
		write(DISK0, supertab->sb_dirlst, sizeof(struct dirblock));
		signal(psuper->sb_freelock);
	}
	
	//now, go with if there is one but it is full

	//release the lock
	signal(psuper->sb_freelock);

	// TODO: Add the block back into the filesystem's list of
 	//  free blocks.  Use the superblock's locks to guarantee
	//  mutually exclusive access to the free list, and write
	//  the changed free list segment(s) back to disk.
	return SYSERR;
}
