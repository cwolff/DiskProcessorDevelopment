/*	Last revised : Sat 12-Sep-87 at 8:45 pm*/
/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ	Disk Processor (C)opyright 1987 Personal Touch Software Inc.	  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
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
 ณ							  init()								  ณ
 ณ																	  ณ
 ณ				  Set up video for use by this program				  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

init()

{

	union REGS inregs;		  /* Used for interrupt registers	 */
	union REGS outregs;

	unsigned int equip_flag;
	char crt_mode;

	/*
	 * Get equipment flag
	 */

	int86(0x0011, &inregs, &outregs);

	equip_flag = outregs.x.ax;

	if ( (VIDEO_STATE & equip_flag) == VIDEO_STATE)
	{
		Hdr_attr = B_BLACK | F_CYAN;
		Hdr_txt_attr = B_BLACK | F_WHITE;
		Tail_attr = B_BLACK | F_BLUE;
		Tail_txt_attr = B_BLACK | F_CYAN;
		Txt_attr = B_BLACK | F_GREEN;
		Error_attr = B_RED | F_BROWN | F_BRIGHT;
		Prompt_attr = B_BLACK | F_BROWN;
		Select_attr = B_WHITE | F_BLACK;

		Video_seg = COLOR_ADDR;
	}
	else
	{
		Hdr_attr = B_BLACK | F_WHITE;
		Hdr_txt_attr = B_BLACK | F_WHITE;
		Tail_attr = B_BLACK | F_WHITE;
		Tail_txt_attr = B_BLACK | F_WHITE;
		Txt_attr = B_BLACK | F_WHITE;
		Error_attr = B_BLACK | F_WHITE | F_BRIGHT;
		Prompt_attr = B_BLACK | F_WHITE;
		Select_attr = B_WHITE | F_BLACK;

		Video_seg = BW_ADDR;
	}

	Number_of_floppies = ((equip_flag & 0x00c0) >> 6) + 1;

	/*
	 * Get the address of the 6845
	 * Get the crt mode setting
	 * Turn off the background blink
	 * Save new crt mode setting
	 */

	PEEK(DATA_SEG, ADDR_6845, &Address_6845, 2);

	PEEK(DATA_SEG, CRT_MODE_SET, &crt_mode, 1);

	crt_mode &= 0x1f;

	/*
	 * Address of Mode Control Register (diable background blink)
	 */

	outp(Address_6845 + 4, crt_mode);

	POKE(DATA_SEG, CRT_MODE_SET, &crt_mode, 1);

	cur_off();

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 							 clr_scrn()					 			  ณ
 ณ																	  ณ
 ณ 						  clear the screen				 			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

clr_scrn()

{

	unsigned short int offset;		/* offset into buffers */

	for (offset = 0; offset < 2000; offset++)
	{
		Scrn_map[offset].character = ' ';
		Scrn_map[offset].attribute = Attribute;
	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 							 clear_w1()				 			  	  ณ
 ณ																	  ณ
 ณ 						  clear window one			 			  	  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

clear_w1()

{

	unsigned short int offset;		/* offset into buffers */

	/*
	 * Clear screen buffer for window 1
	 */

	for (offset = 0; offset < 240; offset++)
	{
		Scrn_map[offset].character = ' ';
		Scrn_map[offset].attribute = Attribute;
	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 							 clear_w2()				 			  	  ณ
 ณ																	  ณ
 ณ 						  clear window two			 			  	  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

clear_w2()

{

	unsigned short int offset;		/* offset into buffers */

	/*
	 * Clear screen buffer for window 2
	 */

	for (offset = 240; offset < 1680; offset++)
	{
		Scrn_map[offset].character = ' ';
		Scrn_map[offset].attribute = Attribute;
	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 							 clear_w3()				 			  	  ณ
 ณ																	  ณ
 ณ 						  restore window three			 			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

clear_w3()

{

	unsigned short int offset;		/* offset into buffers */

	/*
	 * Clear screen buffer for window 3
	 */

	for (offset = 1680; offset < 2000; offset++)
	{
		Scrn_map[offset].character = ' ';
		Scrn_map[offset].attribute = Attribute;
	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 							 save_w1()					 			  ณ
 ณ																	  ณ
 ณ 						  save window one				 			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

save_w1()

{

	unsigned short int offset;		/* offset into buffers */

	/*
	 * Move existing screen buffer for window 1 to the save buffer
	 */

	for (offset = 0; offset < 240; offset++)
	{
		Save_map[offset].character = Scrn_map[offset].character;
		Save_map[offset].attribute = Scrn_map[offset].attribute;
	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 							 save_w2()					 			  ณ
 ณ																	  ณ
 ณ 						  save window two				 			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

save_w2()

{

	unsigned short int offset;		/* offset into buffers */

	/*
	 * Move existing screen buffer for window 2 to the save buffer
	 */

	for (offset = 240; offset < 1680; offset++)
	{
		Save_map[offset].character = Scrn_map[offset].character;
		Save_map[offset].attribute = Scrn_map[offset].attribute;
	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 							 save_w3()					 			  ณ
 ณ																	  ณ
 ณ 						  save window two				 			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

save_w3()

{

	unsigned short int offset;		/* offset into buffers */

	/*
	 * Move existing screen buffer for window 3 to the save buffer
	 */

	for (offset = 1680; offset < 2000; offset++)
	{
		Save_map[offset].character = Scrn_map[offset].character;
		Save_map[offset].attribute = Scrn_map[offset].attribute;
	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 							 restore_w1()				 			  ณ
 ณ																	  ณ
 ณ 						  restore window one			 			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

restore_w1()

{

	unsigned short int offset;		/* offset into buffers */

	/*
	 * Move existing save buffer for window 1 to the screen buffer
	 */

	for (offset = 0; offset < 240; offset++)
	{
		Scrn_map[offset].character = Save_map[offset].character;
		Scrn_map[offset].attribute = Save_map[offset].attribute;
	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 							 restore_w2()				 			  ณ
 ณ																	  ณ
 ณ 						  restore window two			 			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

restore_w2()

{

	unsigned short int offset;		/* offset into buffers */

	/*
	 * Move existing save buffer for window 2 to the screen buffer
	 */

	for (offset = 240; offset < 1680; offset++)
	{
		Scrn_map[offset].character = Save_map[offset].character;
		Scrn_map[offset].attribute = Save_map[offset].attribute;
	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 							 restore_w3()				 			  ณ
 ณ																	  ณ
 ณ 						  restore window three			 			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

restore_w3()

{

	unsigned short int offset;		/* offset into buffers */

	/*
	 * Move existing save buffer for window 3 to the screen buffer
	 */

	for (offset = 1680; offset < 2000; offset++)
	{
		Scrn_map[offset].character = Save_map[offset].character;
		Scrn_map[offset].attribute = Save_map[offset].attribute;
	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ								print_scrn()						  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

print_scrn()

{

	struct SREGS segregs;

	/*
	 * Determine DS (Data Segment)
	 */

	segread(&segregs);

	if (Video_seg == COLOR_ADDR)
	{
		SHOW_VID(segregs.ds,&Scrn_map[0].character,Video_seg,0,80 * 25 * 2);
	}
	else
	{
		movedata(segregs.ds,&Scrn_map[0].character,Video_seg,0,80 * 25 * 2);
 	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ							print_w1()								  ณ
 ณ																	  ณ
 ณ		  Move rows 0, 1, 2 from video memory map to screen memory	  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

print_w1()

{

	struct SREGS segregs;			 /* Used for segment registers */

	/*
	 * Determine Data Segment
	 */

	segread(&segregs);

	if (Video_seg == COLOR_ADDR)
	{
		SHOW_VID(segregs.ds,&Scrn_map[0].character,Video_seg,0,80 * 3);
	}
	else
	{
		movedata(segregs.ds,&Scrn_map[0].character,Video_seg,0,80 * 3 * 2);
 	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 							print_w2()							  	  ณ
 ณ																	  ณ
 ณ		Move rows 3 - 21 from video memory map to screen memory		  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

print_w2()

{

	struct SREGS segregs;			 /* Used for segment registers */

	/*
	 * Determine DS (Data Segment)
	 */

	segread(&segregs);

	if (Video_seg == COLOR_ADDR)
	{
		SHOW_VID(segregs.ds,&Scrn_map[80 * 3].character,Video_seg,80 * 3 * 2,80 * 21);
	}
	else
	{
		movedata(segregs.ds,&Scrn_map[80 * 3].character,Video_seg,80 * 3 * 2,80 * 21 * 2);
 	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ							print_w3()								  ณ
 ณ																	  ณ
 ณ	  Move rows 22, 23, 24 from video memory map to screen memory	  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

print_w3()

{

	struct SREGS segregs;			 /* Used for segment registers */

	/*
	 * Determine DS (Data Segment)
	 */

	segread(&segregs);

	if (Video_seg == COLOR_ADDR)
	{
		SHOW_VID(segregs.ds,&Scrn_map[80 * 22].character,Video_seg,80 * 22 * 2,80 * 3);
	}
	else
	{
		movedata(segregs.ds,&Scrn_map[80 * 22].character,Video_seg,80 * 22 * 2,80 * 3 * 2);
 	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ							print_row(row)							  ณ
 ณ																	  ณ
 ณ				  Print a given row from the screen map		  		  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

print_row(row)

unsigned int row;

{

	struct SREGS segregs;			 /* Used for segment registers */

	/*
	 * Determine Data Segment
	 */

	segread(&segregs);

	if (Video_seg == COLOR_ADDR)
	{
		SHOW_VID(segregs.ds,&Scrn_map[row * 80].character,Video_seg,row * 80 * 2,80);
	}
	else
	{
		movedata(segregs.ds,&Scrn_map[row * 80].character,Video_seg,row * 80 * 2,80 * 2);
 	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ	 					  print(string)						  		  ณ
 ณ																	  ณ
 ณ	Display a string to the screen at the current cursor position	  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

print(string)

char *string;	  /* Pointer to string to display				*/

{

	unsigned int offset;


	/*
	 * Set attribute and number of character to output to one
	 */

	offset = ((Row_col >> 8) * 80) + (Row_col & 0xff);

	while (*string != '\0')
	{
		Scrn_map[offset].character = *string;
		Scrn_map[offset].attribute = Attribute;
		string++;
		offset++;
	}

	Row_col = ((offset / 80) << 8) | (offset % 80);

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ	 					 locate(position)				 			  ณ
 ณ 																 	  ณ
 ณ 			  Locate the cursor to the position of row,col		 	  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

locate(position)

unsigned int position;		  /* Row and Column to locate cursor		  */

{

	union REGS inregs;		  /* Used for interrupt registers	 */
	union REGS outregs;

	Row_col = position;

	inregs.x.ax = 0x0200;
	inregs.x.bx = 0x0000;
	inregs.x.cx = 0x0000;
	inregs.x.dx = position;
		
	int86(0x0010, &inregs, &outregs);

	Row_col = position;

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 							color(fore_back)			 			  ณ
 ณ																	  ณ
 ณ	  Set the screens foreground and background display attributes	  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

color(back_fore)

unsigned int back_fore;	/* Background Foreground attribute for screen */

{

	Attribute = back_fore;

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ							 cur_off()					  			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

cur_off()

{

	/*
	 * Disable cursor (port address of 6845 index and data register)
	 */

	outp(Address_6845, 10);
	outp(Address_6845 + 1, 0x20);	
	outp(Address_6845, 11);
	outp(Address_6845 + 1, 0x20);

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ							 cur_on()								  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

cur_on()

{

	/*
	 * Enable cursor (port address of 6845 index and data register)
	 */

	outp(Address_6845, 10);
	outp(Address_6845 + 1, 0x06);
	outp(Address_6845, 11);
	outp(Address_6845 + 1, 0x07);

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ							  reset()								  ณ
 ณ																	  ณ
 ณ				Restore video to pre set up state					  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

reset()

{

	char crt_mode;

	/*
	 * Get address of 6845
	 * Get the crt mode setting
	 * Enable background blink
	 * Save old crt mode setting
	 */

	PEEK(DATA_SEG, ADDR_6845, &Address_6845, 2);

	PEEK(DATA_SEG, CRT_MODE_SET, &crt_mode, 1);

	crt_mode |= 0x20;

	outp(Address_6845 + 4, crt_mode);

	cur_on();

}

