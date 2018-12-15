/*
	Last revised : Sat 12-Sep-87 at 0:33 pm
*/
/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ  Disk Processor (C)opyright 1987, Personal Touch Software Inc      ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

/*
 * Templates for special library calls
 */

struct seg_regs				/* Template for Segment Registers			 */
{
	unsigned int s_cs;		/* Segment register CS (Code Segment)		 */
	unsigned int s_ss;		/* Segment register SS (Stack Segment)		 */
	unsigned int s_ds;		/* Segment register DS (Data Segment)		 */
	unsigned int s_es;		/* Segment register ES (Extra Segment)		 */
};

struct intr_regs			/* Template for Interrupt Registers			 */
{
	unsigned int i_ax;		/* Interrupt registers AX (Accumalator)		 */
	unsigned int i_bx;		/* Interrupt registers BX (Accumalator)		 */
	unsigned int i_cx;		/* Interrupt registers CX (Accumalator)		 */
	unsigned int i_dx;		/* Interrupt registers DX (Accumalator)		 */
	unsigned int i_si;		/* Interrupt registers SI (Source Index)	 */
	unsigned int i_di;		/* Interrupt registers DI (Destination Index)*/
	unsigned int i_ds;
	unsigned int i_es;
 };

struct t_scrn_map					  /* Template for video memory map	 */
{
	char character;
	char attribute;
};

struct t_boot_map								  /* Template for boot record */
{
	unsigned char filler_1[3];					  /* code to jump to boot	  */
	unsigned char system_id[8];					  /* system id				  */
	unsigned char bytes_per_sector[2];  		  /* bytes per sector		  */
	unsigned char sectors_per_cluster[1];		  /* sectors per cluster	  */
	unsigned char reserved_sectors[2];		 	  /* reserved sectors		  */
	unsigned char number_of_fat[1];				  /* copies of FAT			  */
	unsigned char number_of_root_entries[2];	  /* root directory entries	  */
	unsigned char total_sectors_per_disk[2];	  /* sectors per disk		  */
	unsigned char format_id[1];					  /* format id				  */
	unsigned char number_sectors_per_fat[2];	  /* sectors per FAT		  */
	unsigned char number_sectors_per_track[2];    /* sectors per track		  */
	unsigned char number_heads[2];				  /* heads per drive		  */
	unsigned char number_special_sectors[2];	  /* special sectors		  */
};

/*
 * Machine dependent information
 */

#define	DATA_SEG			0x40
#define	ADDR_6845			0x63
#define	CRT_MODE_SET		0x65
#define	VIDEO_STATE			0x20
#define	COLOR_ADDR			0xb800
#define	BW_ADDR				0xb000

/*
 * Screen attribute information
 */

#define	F_BLACK 					0x00
#define	F_BLUE  					0x01
#define	F_GREEN 					0x02
#define	F_CYAN  					0x03
#define	F_RED						0x04
#define	F_MAGENT					0x05
#define	F_BROWN 					0x06
#define	F_WHITE 					0x07
#define	F_BRIGHT					0x08

#define	B_BLACK 					0x00
#define	B_BLUE  					0x10
#define	B_GREEN 					0x20
#define	B_CYAN  					0x30
#define	B_RED						0x40
#define	B_MAGENT					0x50
#define	B_BROWN 					0x60
#define	B_WHITE 					0x70
#define	B_BLINK						0x80

/*
 * Define views
 */

#define	SECTOR						0x00
#define	BOOT						0x01
#define	FAT							0x02
#define	DIR							0x03
#define	FILE						0x04

