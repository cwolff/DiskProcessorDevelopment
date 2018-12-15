/*
	Last revised : Sat 12-Sep-87 at 1:59 pm
*/
/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ    Disk Processor tm (C)opyright 1987 Personal Touch Software Inc  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

/*
 * Buffers
 */

extern unsigned char Dta[512];

/*
 * Screen attributes
 */

extern unsigned int Hdr_attr;		/* Header attribute */
extern unsigned int Hdr_txt_attr;  	/* Header text attribute */
extern unsigned int Tail_attr;	  	/* Tail attribute */
extern unsigned int Tail_txt_attr;	/* Tail text attribute */
extern unsigned int Txt_attr;		/* Text attribute */
extern unsigned int Error_attr;		/* Error attribute */
extern unsigned int Prompt_attr;	/* Prompt attribute */
extern unsigned int Select_attr;	/* Select view attribute */

/*
 * Machine dependent info
 */

extern unsigned int Address_6845; 			/* Address of motorola 6845	     */
extern unsigned char Attribute;				/* Screen attribute				 */
extern unsigned int Row_col;				/* Row and Column of screen map  */
extern unsigned int Video_addr;				/* Address of video buffer		 */
extern unsigned int Video_seg;	 			/* Segment of video memory		 */
extern struct t_scrn_map Scrn_map[2000];    /* Video memory map				 */
extern struct t_scrn_map Save_map[2000];	/* Save video memory map		 */

/*
 * Disk drive configurations
 */

extern unsigned int retry;			/* maximum number of retries for disk i/o  */

extern unsigned int Bytes_per_sector;				/* bytes per sector				*/
extern unsigned int Sectors_per_cluster;			/* sectors per cluster			*/
extern unsigned int Reserved_sectors;				/* reserved sectors				*/
extern unsigned int Number_of_fat;					/* number of FAT				*/
extern unsigned int Number_of_root_entries;			/*	number of root entries		*/
extern unsigned int Total_sectors_per_disk;			/* total sectors per disk		*/
extern unsigned int Format_id;						/* format id					*/
extern unsigned int Number_sectors_per_fat;			/* number of sectors per FAT	*/
extern unsigned int Number_sectors_per_track;		/* number sectors per track		*/
extern unsigned int Number_heads;					/* heads per drive				*/
extern unsigned int Number_special_sectors;			/* number of special sectors	*/
extern unsigned int Number_of_floppies;				/* number of floppy drives	    */

extern unsigned int Max_drive;		/* max drives on system				   */
extern unsigned int Max_head;	  	/* max heads of drive		  		   */
extern unsigned int Max_track;	 	/* max track on head of disk	 	   */
extern unsigned int Max_sector;		/* max sector of track on head of disk */

extern unsigned int Curr_drive;			/* current drive				   */
extern unsigned int Curr_head;			/* current head					   */
extern unsigned int Curr_track;			/* current track				   */
extern unsigned int Curr_sector;		/* current sector				   */
extern unsigned int Number_of_drives;	/* Number of disk drives on system */

/*
 * Assembly Language Interface Variables and Function Calls
 */

extern int SHOW_VID();
extern int PEEK();
extern int POKE();
