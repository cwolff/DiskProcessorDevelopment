/*	Last revised : Sun 13-Sep-87 at 1:15 pm*/
/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ   Disk Processor (C)opyright 1987 Personal Touch Software Inc	  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

/*
 * Include files
 */

#include "dp.h"
#include "dp_extrn.h"

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ						  print_first_menu()						  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

print_first_menu(view)

unsigned int view;		/* Current disk view */

{

   unsigned char key_t[5];		/* Top of key prompt		 */
   unsigned char key_b[5];		/* Bottom of key prompt		 */
   unsigned char key_str[5];	/* Key prompt				 */
   unsigned int loop;			/* Loop Counter		 		 */

	/*
	 * Display Header
	 */

	print_header("D i s k   P r o c e s s o r", "A Disk Editing Tool");

	print_w1();

	/*
	 * Display Trailer
	 */

	switch(view)
	{
		case 0:
		{
			print_trailer("Sector View","ESC to EXIT");
			break;
		}

		case 1:
		{
			print_trailer("Boot Record View","ESC to EXIT");
			break;
		}

		case 2:
		{
			print_trailer("File Allocation Table View","ESC to EXIT");
			break;
		}

		case 3:
		{
			print_trailer("Directory View","ESC to EXIT");
			break;
		}

		case 4:
		{
			print_trailer("Files View","ESC to EXIT");
			break;
		}

	}

	/*
	 * Display copyright message
	 */

	color(Tail_attr);
	locate(0x1800);
	print("ภฤฤฤ                                                                        ฤฤฤู");

	color(Tail_txt_attr);
	locate(0x1805);
	print("(C) Copyright 1987, Personal Touch Software Inc.   All Rights Reserved");

	print_w3();

	/*
	 * Display menu prompt
	 */

	color(Txt_attr | F_BRIGHT);
	locate(0x0510);
	print("Press the function key next to your choice");

	/*
	 * Display function keys
	 */

	color(B_WHITE | F_BLACK);

	strcpy (key_t, "฿฿฿฿");
	strcpy (key_str, " F  ");
	strcpy (key_b, "ÜÜÜÜ");

	for(loop=0; loop<3; loop++)
	{
		locate(0x0019 | ((7 + (4 * loop)) << 8));
		print(key_t);
		key_str[2] = (char) (0x30 + (1 + loop));
		locate(0x0019 | ((8 + (4 * loop)) << 8));
		print(key_str);
		locate(0x0019 | ((9 + (4 * loop)) << 8));
		print(key_b);
	}

	/*
	 * Display choices
	 */

	color(Txt_attr);

	locate(0x081e);
	print("Select View");

	locate(0x0c1e);
	print("How to use Disk Processor");

	locate(0x101e);
	print("Open Disk");

	print_w2();

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ				  print_menu(f1,f2,f3,f4,f5)						  ณ
 ณ																	  ณ
 ณ		print menu on 25th line with the 5 function texts			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

print_menu(f1,f2,f3,f4,f5)

char *f1;		  /* Soft function key 1					*/
char *f2;		  /* Soft function key 2					*/
char *f3;		  /* Soft function key 3					*/
char *f4;		  /* Soft function key 4					*/
char *f5;		  /* Soft function key 5					*/

{

	char function_key;	 /* Function key symbols		  */
	char key_string[3];	 /* Function Key string			  */
																													  
	color(Tail_attr);
	print("ภ                                                                              ู");


	strcpy (key_string, "F1");
	color(B_BLACK | F_WHITE | F_BRIGHT);
	locate(0x1802);
	print(key_string);

	color(Tail_txt_attr);
	locate(0x1805);
	print(f1);

	strcpy (key_string, "F2");
	color(B_BLACK | F_WHITE | F_BRIGHT);
	locate(0x1811);
	print(key_string);

	color(Tail_txt_attr);
	locate(0x1814);
	print(f2);

	strcpy (key_string, "F3");
	color(B_BLACK | F_WHITE | F_BRIGHT);
	locate(0x1820);
	print(key_string);

	color(Tail_txt_attr);
	locate(0x1823);
	print(f3);

	strcpy (key_string, "F4");
	color(B_BLACK | F_WHITE | F_BRIGHT);
	locate(0x182f);
	print(key_string);

	color(Tail_txt_attr);
	locate(0x1832);
	print(f4);

	strcpy (key_string, "F5");
	color(B_BLACK | F_WHITE | F_BRIGHT);
	locate(0x183e);
	print(key_string);

	color(Tail_txt_attr);
	locate(0x1841);
	print(f5);

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ						print_header(title)							  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

print_header(title, description)

char *title; 					/* Title string						  */
char *description;				/* Screen Description				  */

{

	unsigned char top_frame[81];	/* buffer for the top frame */
	
	/*
	 * Display Header
	 */

	color(B_BLACK | F_WHITE);

	clr_scrn();

	strcpy (top_frame,
		"ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป");

	color(Hdr_attr);

	locate(0x0000);
	print(top_frame);

	strcpy (top_frame,
		"บ                                                                              บ");

	locate(0x0100);
	print(top_frame);

	strcpy (top_frame,
		"ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");

	locate(0x0200);
	print(top_frame);

	color(Hdr_txt_attr | F_BRIGHT);

	locate(0x0102);
	print(title);

	color(Hdr_txt_attr);

	locate(0x0100 | ( 60 - ((strlen(title) & 0x00ff) / 2)));
	print(description);

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ					print_trailer("left,right")						  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

print_trailer(left,right)

char *left;		  /* Left side prompt					  */
char *right;	  /* Right side prompt					  */

{

	unsigned char frame_t[81];	/* Frame string top						*/
	unsigned char frame_m[81];	/* Frame string medium					*/
	unsigned char space[81];	/* Space buffer							*/
	unsigned char line[81];		/* Top line of trailer string			*/
	unsigned int loop;			/* loop counter							*/
	unsigned int left_size;	  	/* Size of left prompt					*/
	unsigned int right_size;	/* Size of right prompt					*/
	unsigned int r_col;			/* column start for right tail			*/

	for (loop = 0; loop < 81; loop++)
	{
		space[loop] = ' ';
		line[loop] = 'อ';
	}

	left_size = strlen(left);
	right_size = strlen(right);

	strcpy (frame_t, "ี[ ");
	strncat (frame_t, space, left_size);
	strcat (frame_t, " ]");

	strncat (frame_t, line, 80 - (strlen(frame_t) + right_size + 5));
	strcat (frame_t, "[ ");
	r_col = strlen(frame_t);
	strncat (frame_t, space, right_size);
	strcat (frame_t, " ]ธ");

	strcpy (frame_m, "ณ                                                                              ณ");

	color(Tail_attr);

	locate(0x1600);
	print(frame_t);

	color(Tail_txt_attr);

	locate(0x1603);
	print(left);

	locate(0x1600 | r_col);
	print(right);

	color (Tail_attr);

	locate(0x1700);
	print(frame_m);

	color(Txt_attr);

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ							print_last_menu()						  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

print_last_menu()

{

	unsigned char key_t[5];		/* Top of key prompt		 */
	unsigned char key_b[5];		/* Bottom of key prompt		 */
	unsigned char key_str[5];	/* Key prompt				 */
	unsigned int loop;			/* Loop Counter				 */

	/*
	 * Display Header
	 */

	print_header("D i s k   P r o c e s s o r", "Sector Edit Exit");

	print_w1();

	/*
	 * Display Trailer
	 */

	print_trailer("Last Choice","ESC to STOP");

	/*
	 * Display copyright message
	 */

	color(Tail_attr);
	locate(0x1800);
	print("ภฤฤฤ                                                                        ฤฤฤู");

	color(Tail_txt_attr);
	locate(0x1805);
	print("(C) Copyright 1987, Personal Touch Software Inc.	All Rights Reserved");

	print_w3();

	/*
	 * Display menu prompt
	 */

	color(Txt_attr | F_BRIGHT);
	locate(0x0510);
	print("Press the function key next to your choice");

	/*
	 * Display function keys
	 */

	color(B_WHITE | F_BLACK);

	strcpy (key_t, "฿฿฿฿");
	strcpy (key_str, " F  ");
	strcpy (key_b, "ÜÜÜÜ");

	for(loop=0; loop<2; loop++)
	{
		locate(0x0019 | ((7 + (4 * loop)) << 8));
		print(key_t);
		key_str[2] = (char) (0x30 + (1 + loop));
		locate(0x0019 | ((8 + (4 * loop)) << 8));
		print(key_str);
		locate(0x0019 | ((9 + (4 * loop)) << 8));
		print(key_b);
	}

	/*
	 * Display choices
	 */

	color(Txt_attr);

	locate(0x081e);
	print("Go to the First Menu");

	locate(0x0c1e);
	print("Oops, continue what I was doing");

	print_w2();

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ							select_view(view)						  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

select_view(view)

unsigned short int *view;		/* Current view of the disk */

{

	unsigned int key_hit;		/* Both control and ascii keyboard		   */
	unsigned int key_ah;		/* Control code from keyboard			   */
	unsigned int key_al;		/* Ascii code from keyboard			   	   */

	/*
	 * Set up header
	 */

	color(B_BLACK | F_WHITE);
	clr_scrn();

	print_header("D i s k   P r o c e s s o r", "Selecting a View of the Disk");
	print_w1();

	/*
	 * Display Trailer
	 */

	print_trailer("View Select","ESC to ACCEPT");

	/*
	 * Display copyright message
	 */

	color(Tail_attr);
	locate(0x1800);
	print("ภฤฤฤ                                                                        ฤฤฤู");

	color(Tail_txt_attr);
	locate(0x1805);
	print("(C) Copyright 1987, Personal Touch Software Inc.   All Rights Reserved");
	print_w3();

	color(Txt_attr | F_BRIGHT);
	locate(0x0510);
	print("Use the  and  keys to make your selection.");

	color(F_WHITE | F_BRIGHT);
	locate(0x120c);
	print("ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ");
	locate(0x130c);
	print("ณ                                                        ณ");
	locate(0x140c);
	print("ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู");

	select_view_text(*view);

	print_w2();

	/*
	 * Get function keys until next_key != 1 (ESC pressed)
	 */

	do
	{

		key_hit = get_key();
		key_ah = key_hit >> 8;
		key_al = key_hit & 0xff;

		switch(key_ah)
		{

			/*
			 * Up Arrow
			 */

			case 72 :
			{

				if (*view > 0)
				{
					*view -= 1;
				}
				else
				{
					*view = 4;
				}

				select_view_text(*view);
				print_w2();
				print_w3();

				break;

			}

			/*
			 * Down Arrow
			 */

			case 80 :
			{

				if (*view < 4)
				{
					*view += 1;
				}
				else
				{
					*view = 0;
				}

				select_view_text(*view);
				print_w2();
				print_w3();

				break;

			}

		}

	}
	while(key_ah != 01);

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ						select_view_text(view)						  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

select_view_text(view)

unsigned short int view;		/* Current view of the disk */

{

	color( ((view == SECTOR) ? Select_attr : Txt_attr) );
	locate(0x0810);
	print(" Sectors ");
	
	color( ((view == BOOT) ? Select_attr : Txt_attr) );
	locate(0x0a10);
	print(" Boot Record ");
	
	color( ((view == FAT) ? Select_attr : Txt_attr) );
	locate(0x0c10);
	print(" File Allocation Table ");
	
	color( ((view == DIR) ? Select_attr : Txt_attr) );
	locate(0x0e10);
	print(" Directories ");
	
	color( ((view == FILE) ? Select_attr : Txt_attr) );
	locate(0x1010);
	print(" Files ");

	color(Txt_attr | F_BRIGHT);
	locate(0x130e);

	switch(view)
	{
		case SECTOR	:
		{
			print("View and operate on the disk's sectors.              ");
			break;
		}

		case BOOT :
		{
			print("View and operate on the disk's boot record.          ");
			break;
		}

		case FAT :
		{
			print("View and operate on the disk's file allocation table.");
			break;
		}

		case DIR :
		{
			print("View and operate on the disk's directories.          ");
			break;
		}

		case FILE :
		{
			print("View and operate on the disk's files.                ");
			break;
		}

	}

}

/*
 ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
 บ	   		   	    												  บ
 บ	  			display_error(row,error_message)				      บ
 บ																	  บ
 ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

display_error(row,error_message)

unsigned short int row;			/* Row to start box 			*/
unsigned char *error_message;	/* Pointer to the error message */

{
	unsigned short int key;
 	unsigned short int loop;
	unsigned short int message_lngth;
	unsigned char strng[81];
	unsigned short int box_col;

	save_w2();

	color(Error_attr);

	row = row << 8;
	message_lngth = strlen(error_message);
	box_col = ((80 - (message_lngth + 4)) / 2);

	strng[0] = 'ฺ';

	for(loop=1; loop<message_lngth+3; loop++)
	{
		strng[loop] =  'ฤ';		
	}

	strng[loop] = 'ฟ';
	strng[loop+1] = '\0';

	locate(row | box_col);
	print(strng);

	strng[0] = 'ณ';

	for(loop=1; loop<message_lngth+3; loop++)
	{
		strng[loop] =  ' ';		
	}

	strng[loop] = 'ณ';

	locate(row + 0x0100 | box_col);
	print(strng);
	locate(row + 0x0200 | box_col);
	print(strng);

	locate(row + 0x0100 | (box_col + 2));
	print(error_message);
	locate(row + 0x0200 | 27);
	print("Press any key to continue.");

	strng[0] = 'ภ';

	for(loop=1; loop<message_lngth+3; loop++)
	{
		strng[loop] =  'ฤ';		
	}

	strng[loop] = 'ู';

	locate(row + 0x0300 | box_col);
	print(strng);

 	print_w2();

	key = get_key() >> 8;

	restore_w2();

	print_w2();

}
