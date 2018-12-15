/*	Last revised : Sat 12-Sep-87 at 1:55 pm*/
/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ    Disk Processor (C)opyright 1987 Personal Touch Software Inc.    ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

/*
 * Include file
 */

#include "dp.h"

/*
 * Gloabal storage
 */

unsigned char Dta[512];    			/* Disk Transfer Area */
unsigned int Address_6845; 			/* Address of motorola 6845 */
unsigned char Attribute;   			/* Screen attribute */
unsigned int Row_col;      			/* Row and Column of screen map */
unsigned int Video_addr;   			/* Address of video buffer */
unsigned int Video_seg;    			/* Segment of video memory */
struct t_scrn_map Scrn_map[2000];   /* Video memory map */
struct t_scrn_map Save_map[2000];	/* Save video memory map */

/*
 * screen attributes
 */

unsigned int Hdr_attr;			/* Header attribute */
unsigned int Hdr_txt_attr;		/* Header text attribute */
unsigned int Tail_attr;			/* Tail attribute */
unsigned int Tail_txt_attr;	/* Tail text attribute */
unsigned int Txt_attr;			/* Text attribute */
unsigned int Error_attr;		/* Error attribute */
unsigned int Prompt_attr;		/* Prompt attribute */
unsigned int Select_attr;		/* Select view attribute */

/*
 * Disk drive configurations
 */

unsigned int retry;				/* maximum number of retries for disk i/o  */

unsigned int Bytes_per_sector;				/* bytes per sector			   */
unsigned int Sectors_per_cluster;			/* sectors per cluster		   */
unsigned int Reserved_sectors;				/* reserved sectors			   */
unsigned int Number_of_fat;					/* number of FAT			   */
unsigned int Number_of_root_entries;		/*	number of root entries	   */
unsigned int Total_sectors_per_disk;		/* total sectors per disk	   */
unsigned int Format_id;						/* format id				   */
unsigned int Number_sectors_per_fat;		/* number of sectors per FAT   */
unsigned int Number_sectors_per_track;		/* number sectors per track	   */
unsigned int Number_heads;					/* heads per drive			   */
unsigned int Number_special_sectors;		/* number of special sectors   */
unsigned int Number_of_floppies;			/* number of floppy drives     */

unsigned int Max_drive;					/* max drives on system				*/
unsigned int Max_head;     			/* max heads of drive        			*/
unsigned int Max_track;    			/* max track on head of disk    		*/
unsigned int Max_sector;   			/* max sector of track on head of disk  */
unsigned int Max_drive;				/* max drive on system					*/

unsigned int Curr_drive;				/* current drive				   	*/
unsigned int Curr_head;					/* current head					    */
unsigned int Curr_track;				/* current track				    */
unsigned int Curr_sector;				/* current sector				    */

