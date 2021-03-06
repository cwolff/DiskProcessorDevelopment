/*	Last revised : Sat 12-Sep-87 at 8:45 pm*/
/*
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³	Disk Processor (C)opyright 1987 Personal Touch Software Inc.	  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
*/

/*
 * Include files
 */

#include "dp.h"
#include "dp_extrn.h"
#include "dos.h"

/*
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³							  init()								  ³
 ³																	  ³
 ³				  Set up video for use by this program				  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³ 							 clr_scrn()					 			  ³
 ³																	  ³
 ³ 						  clear the screen				 			  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³ 							 clear_w1()				 			  	  ³
 ³																	  ³
 ³ 						  clear window one			 			  	  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³ 							 clear_w2()				 			  	  ³
 ³																	  ³
 ³ 						  clear window two			 			  	  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³ 							 clear_w3()				 			  	  ³
 ³																	  ³
 ³ 						  restore window three			 			  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³ 							 save_w1()					 			  ³
 ³																	  ³
 ³ 						  save window one				 			  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³ 							 save_w2()					 			  ³
 ³																	  ³
 ³ 						  save window two				 			  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³ 							 save_w3()					 			  ³
 ³																	  ³
 ³ 						  save window two				 			  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³ 							 restore_w1()				 			  ³
 ³																	  ³
 ³ 						  restore window one			 			  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³ 							 restore_w2()				 			  ³
 ³																	  ³
 ³ 						  restore window two			 			  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³ 							 restore_w3()				 			  ³
 ³																	  ³
 ³ 						  restore window three			 			  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³								print_scrn()						  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³							print_w1()								  ³
 ³																	  ³
 ³		  Move rows 0, 1, 2 from video memory map to screen memory	  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³ 							print_w2()							  	  ³
 ³																	  ³
 ³		Move rows 3 - 21 from video memory map to screen memory		  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³							print_w3()								  ³
 ³																	  ³
 ³	  Move rows 22, 23, 24 from video memory map to screen memory	  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³							print_row(row)							  ³
 ³																	  ³
 ³				  Print a given row from the screen map		  		  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³	 					  print(string)						  		  ³
 ³																	  ³
 ³	Display a string to the screen at the current cursor position	  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³	 					 locate(position)				 			  ³
 ³ 																 	  ³
 ³ 			  Locate the cursor to the position of row,col		 	  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³ 							color(fore_back)			 			  ³
 ³																	  ³
 ³	  Set the screens foreground and background display attributes	  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
*/

color(back_fore)

unsigned int back_fore;	/* Background Foreground attribute for screen */

{

	Attribute = back_fore;

}

/*
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³							 cur_off()					  			  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³							 cur_on()								  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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
 ΥΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΈ
 ³																	  ³
 ³							  reset()								  ³
 ³																	  ³
 ³				Restore video to pre set up state					  ³
 ³																	  ³
 ΤΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΝΎ
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