/* fileDelete.c - fileDelete */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*                                                                 */
/* and                                                             */
/*                                                                 */
/*                                                                 */

#include <kernel.h>
#include <memory.h>
#include <file.h>

/*------------------------------------------------------------------------
 * fileDelete - Delete a file.
 *------------------------------------------------------------------------
 */
devcall fileDelete(int fd)
{
	
	if ((NULL == supertab) || (NULL == filetab))
	{
		return SYSERR;
	}
	
	//look for function seek and wait
	//grab lock
	wait(supertab->sb_dirlock);

	//check to see if the number of entries is less than the called delete
	if(fd >= DIRENTRIES)
	{
		signal(supertab->sb_dirlock);
		return SYSERR;
	}
	
	//use sbFreeBlock to free up the block
	supertab->sb_dirlst->db_fnodes->fn_state = FILE_FREE;
	sbFreeBlock(supertab, fd);

	//overwrite the block on disk
	seek(DISK0, supertab->sb_dirlst->db_blocknum);
	write(DISK0, supertab->sb_dirlst, sizeof(struct dirblock));

	if(SYSERR == filetab[fd].fn_blocknum)
	{
		return SYSERR;
	}

	//release the lock
	signal(supertab->sb_dirlock);
	
	// TODO: Unlink this file from the master directory index,
	//  and return its space to the free disk block list.
 	//  Use the superblock's locks to guarantee mutually exclusive
 	//  access to the directory index.
 	return OK;
}
