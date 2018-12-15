/*	Last revised : Sun 13-Sep-87 at 1:13 pm*/
/*
 ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
 บ																	  บ
 บ  Disk Processor tm (C)opyright 1987 Personal Touch Software Inc.	  บ
 บ																	  บ
 ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

/*
 * Include files
 */

#include "dp.h"
#include "dp_extrn.h"
#include "dos.h"

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ							  reset_disk()							  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

reset_disk()

{

	union REGS inregs;				 /* Used for interrupt registers	*/
	union REGS outregs;
	struct SREGS segregs;

	/*
	 * Determine DS (Data Segment)
	 */

	segread(&segregs);

	/*
	 * Reset the drive
	 */

	inregs.x.ax = 0x0000;

	int86x(0x13, &inregs, &outregs, &segregs);

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ			  read_sector(drive,side,track,sector,cdta)				  ณ
 ณ																	  ณ
 ณ			 Read disk in drive for side track sector into cdta		  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

read_sector(drive,head,track,sector,cdta)

unsigned int *drive;		  /* Drive selector (A = 0, B = 1, etc)		*/
unsigned int *head;			  /* Disk head (0 to Max_head)				*/
unsigned int *track;		  /* Disk track (0 to Max_track)			*/
unsigned int *sector;		  /* Disk sector (1 to Max_sector)			*/
char *cdta;					  /* Pointer to current DTA					*/

{

	struct SREGS segregs;			 /* Used for segment registers		*/
	union REGS inregs;				 /* Used for interrupt registers	*/
	union REGS outregs;

	/*
	 * Update global variables
	 */

	Curr_drive = *drive;
	Curr_head = *head;
	Curr_track = *track;
	Curr_sector = *sector;

	/*
	 * Reset the drive as a precaution
	 */

	reset_disk();

	/*
	 * If the drive is a floppy drive then use int 13h to
	 * by pass the DOS prompts
	 *
	 * Since this is a floppy disk we must read the boot
	 * record to make sure we can adjust values if the
	 * disk has been swapped.
	 */

	read_boot_record(*drive);

	/*
	 * Determine DS (Data Segment)
	 */

	segread(&segregs);

	if (*drive == 0 || *drive == 1)
	{

		/*
		 * Check to see if the boot record was read
		 */

		if (Max_track == 0 || Max_sector ==0)
		{
			return(1);
		}
		else
		{

			/*
			 * If the format of the disk is different than what was
			 * know at the time of the last disk operation we will
		 	* read the disk anyway after adjusting the values.
		 	*/

			if (*head > Max_head || *track > Max_track || *sector > Max_sector)
			{
				display_error(17,"The specifed sector is not a valid sector for this diskette.");
				return(0);
			}
			else
			{
				/*
				 * Set up interrupt registers for
				 * Reading * sectors
				 * into the dta
				 * starting at track 0 sector 1
				 * from drive 0 head 0
				 * ES = DS
				 */
		
				inregs.x.ax = 0x0201;
				inregs.x.bx = (int) cdta;
				inregs.x.cx = ((*track << 8) | *sector);
		
				if (Number_of_floppies < 2)
				{
					inregs.x.dx = ((*head << 8) | 0);
				}
				else
				{
					if (*drive == 0)
					{
						inregs.x.dx = ((*head << 8) | 0);
					}
					else
					{
						inregs.x.dx = ((*head << 8) | 1);
					}
				}
	
				segregs.es = segregs.ds;
		
				int86x(0x13, &inregs, &outregs, &segregs);
		
				/*
				 * return status of disk operation
				 */
		
				return(outregs.x.ax >> 8);
			}
		}
	}
	else
	{

		/*
		 * Set up interrupt registers for
		 * Reading * sectors
		 * into the dta
		 * starting at track 0 sector 1
		 * from drive 0 head 0
		 * ES = DS
		 */
	
		inregs.x.ax = *drive;
		inregs.x.bx = (int) cdta;
		inregs.x.cx = 1;
		inregs.x.dx = (*sector - 1) + *head * Number_sectors_per_track + *track * Number_sectors_per_track * Number_heads;
		segregs.es = segregs.ds;

		int86x(0x25, &inregs, &outregs, &segregs);
	
		/*
		 * return status of disk operation
		 */
	
		return(outregs.x.ax & 0x00ff);
	
	}
}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ	   					  write_sector()					  		  ณ
 ณ																	  ณ
 ณ	   			  Write current dta to specified sector		  	  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

write_sector(drive,head,track,sector,cdta)

unsigned int *drive;			/* Drive selector (A = 0, B = 1, etc)			*/
unsigned int *head;				/* Disk head (0 to Max_head)				*/
unsigned int *track;			/* Disk track (0 to Max_track)				*/
unsigned int *sector;			/* Disk sector (1 to Max_sector)			*/
char *cdta;						/* Pointer to current DTA					*/

{

	struct SREGS segregs;			 /* Used for segment registers			*/
	union REGS inregs;				 /* Used for interrupt registers		*/
	union REGS outregs;

	/*
	 * Update global variables
	 */

	Curr_drive = *drive;
	Curr_head = *head;
	Curr_track = *track;
	Curr_sector = *sector;

	/*
	 * Reset the drive as a precaution
	 */

	reset_disk();

	/*
	 * If the drive is a floppy drive then use int 13h to
	 * by pass the DOS prompts
	 *
	 * Since this is a floppy disk we must read the boot
	 * record to make sure we can adjust values if the
	 * disk has been swapped.
	 */

	read_boot_record(*drive);

	/*
	 * Determine DS (Data Segment)
	 */

	segread(&segregs);

	if (*drive == 0 || *drive == 1)
	{

		/*
		 * Check to see if the boot record was read
		 */

		if (Max_track == 0 || Max_sector == 0)
		{
			return(1);
		}
		else
		{

			/*
			 * If the format of the disk is different than what was
			 * know at the time of the last disk operation we will
			 * not write to the disk.
			 */
	
			if (*head > Max_head || *track > Max_track || *sector > Max_sector)
			{
				display_error(17,"The specifed sector is not a valid sector for this diskette.");
	
				return(0);
			}
			else
			{
	
				/*
				 * Set up interrupt registers for
				 * Writting * sectors
				 * from the dta
				 * starting at track 0 sector 1
				 * from drive 0 head 0
				 * ES = DS
				 */
	
				inregs.x.ax = 0x0301;
				inregs.x.bx = (int) cdta;
				inregs.x.cx = ((*track << 8) | *sector);
	
				if (Number_of_floppies < 2)
				{
					inregs.x.dx = ((*head << 8) | 0);
				}
				else
				{
					if (*drive == 0)
					{
						inregs.x.dx = ((*head << 8) | 0);
					}
					else
					{
						inregs.x.dx = ((*head << 8) | 1);
					}
				}
		
				segregs.es = segregs.ds;
	
				int86x(0x13, &inregs, &outregs, &segregs);
	
				/*
				 * return status of disk operation
				 */
	
				return(outregs.x.ax >> 8);
			}
		}
	
	}
	else
	{

		/*
		 * Set up interrupt registers for writing sectors into the dta
		 * starting at track, sector, head for drive
		 * ES = DS
		 */
	
		inregs.x.ax = *drive;
		inregs.x.bx = (int) cdta;
		inregs.x.cx = 1;
		inregs.x.dx = (*sector - 1) + *head * Number_sectors_per_track + *track * Number_sectors_per_track * Number_heads;
		segregs.es = segregs.ds;
	
		int86x(0x26, &inregs, &outregs, &segregs);
	
		/*
		 * return status of disk operation
		 */
	
		return(outregs.x.ax & 0x00ff);
	
	}
}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ		 				  read_boot_record() 						  ณ
 ณ																	  ณ
 ณ	  Read the boot record and initialize the disk parameters		  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

read_boot_record(drive)

char drive;

{

 	struct t_boot_map *boot_ptr;	/* Pointer to boot record		*/
	unsigned short int *int_ptr_1;	/* Pointer to integer			*/
	unsigned short int disk_status;	/* disk return status			*/
	unsigned cdta[512];				/* disk transfer addrss			*/
	struct SREGS segregs;			/* Used for segment registers	*/
	union REGS inregs;				/* Used for interrupt registers	*/
	union REGS outregs;

	/*
	 * Disk initialization
	 */

	retry = 0;
	disk_status = 1;

	/*
	 * Determine DS (Data Segment)
	 */

	segread(&segregs);

	while ( (retry++ < 3) && (disk_status > 0) )
	{

		/*
		 * If the drive is a floopy drive then use int 13h to
		 * by pass the DOS prompts
		 */
	
		if (drive == 0 || drive == 1)
		{
	
			/*
			 * Set up interrupt registers for
			 * Reading * sectors
			 * into the dta
			 * starting at track 0 sector 1
			 * from drive 0 head 0
			 * ES = DS
			 */
	
			inregs.x.ax = 0x0201;
			inregs.x.bx = (int) cdta;
			inregs.x.cx = 0x0001;

			if (Number_of_floppies < 2)
			{
				inregs.x.dx = 0;
			}
			else
			{
				if (drive == 0)
				{
					inregs.x.dx = 0;
				}
				else
				{
					inregs.x.dx = 1;
				}
			}

			segregs.es = segregs.ds;
	
			int86x(0x13, &inregs, &outregs, &segregs);
	
			/*
			 * set status of disk operation
			 */
	
			disk_status = (outregs.x.ax >> 8);
	
		}
		else
		{
	
			/*
			 * Set up interrupt registers for
			 * Reading * sectors
			 * into the dta
			 * starting at track 0 sector 1
			 * from drive 0 head 0
			 * ES = DS
			 */
	
			inregs.x.ax = drive;
			inregs.x.bx = (int) cdta;
			inregs.x.cx = 1;
			inregs.x.dx = 0;
			segregs.es = segregs.ds;
	
			int86x(0x25, &inregs, &outregs, &segregs);
	
			/*
			 * set status of disk operation
			 */
	
			disk_status = (outregs.x.ax & 0x00ff);
	
		}
	
	}
		 
	if (disk_status)
	{
		Max_head = 0;
		Max_track = 0;
		Max_sector = 0;
	}
	else
	{
		boot_ptr = (struct t_boot_map *) cdta;	
	
		/*
		 * Had to play games here.
		 * Since the structure boot_ptr is character based (byte and word mix)
		 * it was necessary to get the address of the start of the field
		 * and place it in a integer pointer.
		 */
	
		Sectors_per_cluster = ((unsigned int) boot_ptr->sectors_per_cluster[0]);
	
		Reserved_sectors = (((unsigned int) boot_ptr->reserved_sectors[1]) << 8) | ((unsigned int) boot_ptr->reserved_sectors[0]);
	
		Number_of_fat = ((unsigned int) boot_ptr->number_of_fat[0]);
	
		Number_of_root_entries = (((unsigned int) boot_ptr->number_of_root_entries[1]) << 8) | ((unsigned int) boot_ptr->number_of_root_entries[0]);
	
		Total_sectors_per_disk = (((unsigned int) boot_ptr->total_sectors_per_disk[1]) << 8) | ((unsigned int) boot_ptr->total_sectors_per_disk[0]);
	
		Format_id = ((unsigned int) boot_ptr->format_id);
	
		Number_sectors_per_fat = (((unsigned int) boot_ptr->number_sectors_per_fat[1]) << 8) | ((unsigned int) boot_ptr->number_sectors_per_fat[0]);
	
		Number_sectors_per_track=((unsigned int)boot_ptr->number_sectors_per_track[1] << 8)|((unsigned int)boot_ptr->number_sectors_per_track[0]);
	
		Number_heads = (((unsigned int) boot_ptr->number_heads[1]) << 8) | ((unsigned int) boot_ptr->number_heads[0]);
	
		Number_special_sectors = (((unsigned int) boot_ptr->number_special_sectors[1]) << 8) | ((unsigned int) boot_ptr->number_special_sectors[0]);
	
		/*
		 * Make sure none of the divisors are zero (happens with bad reads)
		 */
	
		if (Number_heads != 0 && Number_sectors_per_track != 0 && Number_heads != 0)
		{
			Max_head = Number_heads - 1;
			Max_track =	((Total_sectors_per_disk / Number_sectors_per_track) / Number_heads) - 1;
			Max_sector = Number_sectors_per_track;
		}
		else
		{
			Max_head = 0;
			Max_track = 0;
			Max_sector = 0;
		}
	
	}
	
}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ					 	check_disk(status,drive)					  ณ
 ณ																	  ณ
 ณ				  Select head track and sector on disk				  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

check_disk(status,drive)

unsigned short int status;
unsigned short int drive;

{
	if (status)
	{
		if (drive == 0 || drive == 1)
		{
			display_error(17,"There is a problem with the drive or the floppy disk, please fix.");
		}
		else
		{
			display_error(17,"There is a fatal problem with the drive, this drive is unusable.");
		}
	}
}

