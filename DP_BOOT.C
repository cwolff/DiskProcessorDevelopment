/* Last revised : Sun 13-Sep-87 at 2:38 pm*/
/*
 ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
 บ																	  บ
 บ   Disk Processor (C)opyright 1987 Personal Touch Software Inc.	  บ
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
 ณ                        boot_record_view()						  ณ
 ณ																	  ณ
 ณ           select, read, write, previous, next sector driver		  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

boot_record_view()

{

	unsigned int key_hit;		/* Both control and ascii keyboard		   */
	unsigned int key_ah;		/* Control code from keyboard			   */
	unsigned int key_al;		/* Ascii code from keyboard			   	   */
	unsigned char next_key;		/* Flag if = 1 then get next key else end  */
	unsigned char valid_key;	/* Flag if = 1 then valid key received	   */
	unsigned int head;			/* Head of disk							   */
	unsigned int track;		  	/* Track on head of disk				   */
	unsigned int sector;		/* Sector of track on head of disk		   */
	unsigned int drive;			/* Disk drive A=0, B=1, C=2, ...		   */
	unsigned int status;		/* Return status from disk operation	   */
	char *show_hex();			/* Convert to hex						   */
	unsigned int prev_next;	 	/* 0 = first page, 1 = second page			   */

	unsigned int row_col;		/* row and col of cursor				   */
	unsigned int offset;		/* offset into Dta						   */
	unsigned int hi_lo;		  	/* hex view : 1 = left char 0 = right char */
	unsigned int view;			/* 1 = hex view 0 = ascii view			   */

	char hex[3];				/* Hex data string					   	   */
	char ascii[2];				/* Ascii data							   */

	struct SREGS segregs;		/* Used for segment registers			   */

	/*
	 * Determine DS (Data Segment)
	 */

	segread(&segregs);

	/*
	 * Initialize the disk
	 */

	/*
	 * Initialize position values
	 */

	head = 0;
	track = 0;
	sector = 1;
	drive = bdos(0x19, 0, 0) & 0x00ff;
	Max_drive = ((bdos(0x0e, &drive, 0) & 0x00ff) - 1);
	read_boot_record(drive);

	/*
	 * Initialize end of hex string
	 */

	hex[1] = '\0';
	hex[2] = '\0';
	ascii[1] ='\0';

	row_col = 0x0509;
	offset = 0;

	/*
	 * High character (1 = high, 0 = low), HEX view (1 = high, 0 = low)
	 */

	hi_lo = 1;
	view = 1;

	/*
	 * set page value
	 */

	prev_next = 0;

	/*
	 * Set up menu and open disk
	 */

	color(B_BLACK | F_WHITE);
	clr_scrn();

	print_header("D i s k   P r o c e s s o r", "Boot Record View");
	show_page(prev_next);
	print_w1();

	print_trailer("Drive  ","Reading Boot Record");
	print_menu("Select","Read","Write"," "," ");
	show_select_ascii(0,0x1608,drive);
	print_w3();

	retry = 0;
	while( (status = read_sector(&drive,&head,&track,&sector,Dta)) && (retry++ < 3))
	;

	check_disk(status,drive);

	print_trailer("Drive  ","ESC when DONE");
	print_menu("Select","Read","Write"," "," ");
	show_select_ascii(0,0x1608,drive);
	print_w3();

	show_sector(Dta,0 + (prev_next * 256));
	print_w2();

	locate(row_col = 0x0509);
	offset = 0;

	/*
	 * High character (1 = high, 0 = low), HEX view (1 = high, 0 = low)
	 */

	hi_lo = 1;
	view = 1;

	/*
	 * Get function keys until next_key != 1 (ESC pressed)
	 */

	next_key = 1;

	do
	{

		cur_on();

		key_hit = get_key();
		key_ah = key_hit >> 8;
		key_al = key_hit & 0xff;

		switch(key_ah)
		{

			/*
			 * ESC key pressed
			 */

			case 01 :
			{

				cur_off();

				color(B_BLACK | F_WHITE);
				clr_scrn();

				print_last_menu();

				valid_key = 0;

				do
				{

					key_hit = get_key();
		  			key_ah = key_hit >> 8;
					key_al = key_hit & 0xff;

					switch(key_ah)
					{

						/*
						 * Yes stop
						 */

						case 01 :
						case 59 :
						case 94 :
						case 84 :
						case 104 :
						{
							next_key = 0;
							valid_key = 1;

							break;
						}

						/*
						 * Oops Continue (F2)
				 		 */

						case 60 :
						case 95 :
						case 85 :
						case 105 :
						{
							color(B_BLACK | F_WHITE);
							clr_scrn();

							print_header("D i s k   P r o c e s s o r", "Boot Record View");
							show_page(prev_next);
							print_w1();

							print_trailer("Drive  ","ESC when DONE");
							print_menu("Select","Read","Write"," "," ");
							show_select_ascii(0,0x1608,drive);
							print_w3();

							show_sector(Dta,0 + (prev_next * 256));
							print_w2();

							locate(row_col);

							cur_on();

							valid_key = 1;

							break;
						}
					}
				}
				while (valid_key != 1);

				break;

			}

			/*
			 * Select Sector (F1)
			 */

			case 59 :
			case 94 :
			case 84 :
			case 104 :
			{

				cur_off();

				/*
				 * Display trailer update
				 */

				print_trailer("Drive  ","ESC to ACCEPT");
				print_menu("Select","Read","Write"," "," ");
				show_select_ascii(0,0x1608,drive);
				print_w3();

				if (select_disk(&drive))
				{
					print_trailer("Drive  ","ESC when DONE");
					print_menu("Select","Read","Write"," "," ");
					show_select_ascii(0,0x1608,drive);
					print_w3();

				}
				else
				{

					print_trailer("Drive  ","Reading Boot Record");
					print_menu("Select","Read","Write"," "," ");
					show_select_ascii(0,0x1608,drive);
					print_w3();
	
					retry = 0;
					while((status = read_sector(&drive,&head,&track,&sector,Dta)) && (retry++ < 3))
					;

					check_disk(status,drive);

					print_trailer("Drive  ","ESC when DONE");
					print_menu("Select","Read","Write"," "," ");
					show_select_ascii(0,0x1608,drive);
					print_w3();

				}
	
				show_sector(Dta,0 + ( prev_next * 256));
				print_w2();
	
				locate(row_col);
	
				cur_on();
	
				break;
			}

			/*
			 * Read Sector	(F2)
			 */

			case 60 :
			case 95 :
			case 85 :
			case 105 :
			{

				cur_off();

				print_trailer("Drive  ","Reading Boot Record");
				print_menu("Select","Read","Write"," "," ");
				show_select_ascii(0,0x1608,drive);
				print_w3();

				retry = 0;
				while((status = read_sector(&drive,&head,&track,&sector,Dta)) && (retry++ < 3))
				;

				check_disk(status,drive);

				print_trailer("Drive  ","ESC when DONE");
				print_menu("Select","Read","Write"," "," ");
				show_select_ascii(0,0x1608,drive);
				print_w3();

				show_sector(Dta,0 + ( prev_next * 256));
				print_w2();

				locate(row_col);

				cur_on();

				break;
			}

			/*
			 * Write Sector (F3)
			 */

			case 61 :
			case 96 :
			case 86 :
			case 106 :
			{

				cur_off();

				print_trailer("Drive  ","Writting Boot Record");
				print_menu("Select","Read","Write"," "," ");
				show_select_ascii(0,0x1608,drive);
				print_w3();

				retry = 0;
				while((status = write_sector(&drive,&head,&track,&sector,Dta)) && (retry++ < 3))
				;

				check_disk(status,drive);

				print_trailer("Drive  ","ESC when DONE");
				print_menu("Select","Read","Write"," "," ");
				show_select_ascii(0,0x1608,drive);
				print_w3();

				locate(row_col);

				cur_on();

				break;
			}

			/*
			 * Void (F4)
			 */

			case 62 :
			case 97 :
			case 87 :
			case 107:
			{
				break;
			}

			/*
			 * Void	(F5)
			 */

			case 63 :
			case 98 :
			case 88 :
			case 108 :
			{
				break;
			}

			/*
			 * Previous Key (PgUp)
			 */

			case 73 :
			case 132 :
			{

				if (prev_next == 1)
				{
					cur_off();

					prev_next = 0;
					offset -= 256;

					show_page (prev_next);
					print_w1();

					show_sector(Dta, ( offset < 256 ? 0 : 256) );
					print_w2();

					locate(row_col);

					cur_on();
				}
				break;
			}

			/*
			 * Next Key	(PgDn)
			 */

			case 81 :
			case 118 :
			{

				if (prev_next == 0)
				{
					cur_off();

					prev_next = 1;
					offset += 256;

					show_page(prev_next);
					print_w1();

					show_sector(Dta, ( offset < 256 ? 0 : 256) );
					print_w2();

					locate(row_col);

					cur_on();
				}
				break;
			}

			/*
			 * Begin Key (Home)
			 */

			case 71 :
			case 119 :
			{

				/*
				 * Determine if home key or begin key
				 */

				if (key_ah == 119)
				{

					/*
					 * Home key hit
					 */

					if (prev_next != 0)
					{
						cur_off();

						prev_next = 0;
						show_page (prev_next);
						print_w1();

						show_sector(Dta,0);
						print_w2();

						cur_on();
					}

					offset = 0;

				}
				else
				{

					/*
					 * Begin key hit
					 */

					if (prev_next)
					{

						/*
						 * Second page
						 */

						offset = 256;

					}
					else
					{

						/*
						 * First page
						 */

						offset = 0;

					}

				}

				/*
				 * If HEX view (1 = HEX, 0 = ASCII)
				 */

				if (view)
				{
					hi_lo = 1;
					row_col = 0x0509;
				}
				else
				{
					row_col = 0x053e;
				}

				locate(row_col);

				break;
			}

			/*
			 * End Key (End)
			 */

			case 79 :
			case 117 :
			{

				/*
				 * Determine if end or end home was hit
				 */

				if (key_ah == 117)
				{

					/*
					 * End home was hit
					 */

					if (prev_next != 1)
					{
						cur_off();

						prev_next = 1;
						show_page(prev_next);
						print_w1();

						show_sector(Dta,256);
						print_w2();

						cur_on();
					}

					offset = 511;

				}
				else
				{

					/*
					 * End was hit
					 */

					if (prev_next)
					{

						/*
						 * Second Page
						 */

						offset = 511;

					}
					else
					{

						/*
						 * First Page
						 */

						offset = 255;

					}

				 }

				/*
				 * If HEX view (1 = HEX, 0 = ASCII)
				 */

				if (view)
				{
					hi_lo = 0;
					row_col = 0x1400 + 57;
				}
				else
				{
					row_col = 0x1400 + 77;
				}

				locate(row_col);

				break;
			}

			/*
			 * Up Arrow
			 */

			case 72 :
			{

				if ((row_col & 0xff00) == 0x0500)
				{
					if (prev_next == 0)
					{
						prev_next = 1;
						offset += 496;
					}
					else
					{
						prev_next = 0;
						offset -= 16;
					}

					cur_off();

					show_page(prev_next);
					print_w1();

					show_sector(Dta, ( offset < 256 ? 0 : 256) );
					print_w2();

					cur_on();

					locate(row_col = 0x1400 | (row_col & 0x00ff));
				}
				else
				{
					locate(row_col = row_col - 0x0100);
					offset -= 16;
				}
				break;
			}

			/*
			 * Right Arrow
			 */

			case 77 :
			{

				switch(row_col & 0x00ff)
				{

					/*
					 * High characters of left bank of HEX view
					 */

					case 9 :
					case 12 :
					case 15 :
					case 18 :
					case 21 :
					case 24 :
					case 27 :
					case 30 :
					{
						locate(row_col = (row_col + 0x0001));
						hi_lo = 0;
						break;
					}

					/*
					 * Low characters of left bank of HEX view
					 */

					case 10 :
					case 13 :
					case 16 :
					case 19 :
					case 22 :
					case 25 :
					case 28 :
					{
						locate(row_col = (row_col + 0x0002));
						offset++;
						hi_lo = 1;
						break;
					}

					/*
					 * Low character of last field of left bank of HEX view
					 */

					case 31 :
					{
						locate(row_col = (row_col & 0xff00) + 35);
						offset++;
						hi_lo = 1;
						break;
					}

					/*
					 * High characters of right bank of HEX view
					 */

					case 35 :
					case 38 :
					case 41 :
					case 44 :
					case 47 :
					case 50 :
					case 53 :
					case 56 :
					{
						locate(row_col = (row_col + 0x0001));
						hi_lo = 0;
						break;
					}

					/*
					 * Low characters of right bank of HEX view
					 */

					case 36 :
					case 39 :
					case 42 :
					case 45 :
					case 48 :
					case 51 :
					case 54 :
					{
						locate(row_col = (row_col + 0x0002));
						offset++;
						hi_lo = 1;
						break;
					}

					/*
					 * Low character of last field of right bank of HEX view
					 */

					case 57 :
					{
						if ((row_col & 0xff00) == 0x1400)
						{
							if (prev_next == 1)
							{
								prev_next = 0;
								offset = 0;
							}
							else
							{
								prev_next = 1;
								offset = 256;
							}

							cur_off();

							show_page(prev_next);
							print_w1();

							show_sector(Dta,offset);
							print_w2();

							cur_on();

							row_col = 0x0500 | (row_col & 0x00ff);
						}
						else
						{
							row_col = row_col + 0x0100;
							offset++;
						}

						hi_lo = 1;

						locate(row_col = (row_col & 0xff00) + 9);
						break;
					}

					/*
					 * Characters of ASCII view
					 */

					case 62 :
					case 63 :
					case 64 :
					case 65 :
					case 66 :
					case 67 :
					case 68 :
					case 69 :
					case 70 :
					case 71 :
					case 72 :
					case 73 :
					case 74 :
					case 75 :
					case 76 :
					{
						locate(row_col = (row_col + 0x0001));
						offset++;
						break;
					}

					/*
					 * Last character of ASCII view
					 */

					case 77 :
					{
						if ((row_col & 0xff00) == 0x1400)
						{
							if (prev_next == 1)
							{
								prev_next = 0;
								offset = 0;
							}
							else
							{
								prev_next = 1;
								offset = 256;
							}

							cur_off();

							show_page(prev_next);
							print_w1();

							show_sector(Dta,offset);
							print_w2();

							cur_on();

							row_col = 0x0500 | (row_col & 0x00ff);
						}
						else
						{
							row_col = row_col + 0x0100;
							offset++;
						}

						locate(row_col = (row_col & 0xff00) + 62);
						break;
					}

					default :
					{
						break;
					}

				}
				break;
			}

			/*
			 * Down Arrow
			 */

			case 80 :
			{

				if ((row_col & 0xff00) == 0x1400)
				{
					if (prev_next == 1)
					{
						prev_next = 0;
						offset -= 496;
					}
					else
					{
						prev_next = 1;
						offset += 16;
					}

					cur_off();

					show_page(prev_next);
					print_w1();

					show_sector(Dta, ( offset < 256 ? 0 : 256) );
					print_w2();

					cur_on();

					locate(row_col = 0x0500 | (row_col & 0x00ff));

				}
				else
				{
					locate(row_col = row_col + 0x0100);
					offset += 16;
				}
				break;
			}

			/*
			 * Left Arrow
			 */

			case 75 :
			{
 
				switch(row_col & 0x00ff)
				{

					/*
					 * High character of first field of left bank of HEX view
					 */

					case 9 :
					{
						if ((row_col & 0xff00) == 0x0500)
						{
							if (prev_next == 0)
							{
								prev_next = 1;
								offset = 256;
							}
							else
							{
								prev_next = 0;
								offset = 0;
							}

							cur_off();

							show_page(prev_next);
							print_w1();

							show_sector(Dta,offset);
							print_w2();

							cur_on();

							row_col = 0x1400 | (row_col & 0x00ff);
						}
						else
						{
							row_col = row_col - 0x0100;
							offset--;
						}

						hi_lo = 0;

						locate(row_col = (row_col & 0xff00) + 57);
						break;

					}

					/*
					 * Low characters of left bank of HEX view
					 */

					case 10 :
					case 13 :
					case 16 :
					case 19 :
					case 22 :
					case 25 :
					case 28 :
					case 31 :
					{
						locate(row_col = (row_col - 0x0001));
						hi_lo = 1;
						break;
					}

					/*
					 * High characters of left bank of HEX view
					 */

					case 12 :
					case 15 :
					case 18 :
					case 21 :
					case 24 :
					case 27 :
					case 30 :
					{
						locate(row_col = (row_col - 0x0002));
						offset--;
						hi_lo = 0;
						break;
					}

					/*
					 * High character of first field of right bank of HEX view
					 */

					case 35 :
					{
						locate(row_col = (row_col & 0xff00) + 31);
						offset--;
						hi_lo = 0;
						break;
					}

					/*
					 * Low characters of right bank of HEX view
					 */

					case 36 :
					case 39 :
					case 42 :
					case 45 :
					case 48 :
					case 51 :
					case 54 :
					case 57 :
					{
						locate(row_col = (row_col - 0x0001));
						hi_lo = 1;
						break;
					}

					/*
					 * High characters of left bank of HEX view
					 */

					case 38 :
					case 41 :
					case 44 :
					case 47 :
					case 50 :
					case 53 :
					case 56 :
					{
						locate(row_col = (row_col - 0x0002));
						offset--;
						hi_lo = 0;
						break;
					}

					/*
					 * First character of ASCII view
					 */

					case 62 :
					{
						if ((row_col & 0xff00) == 0x0500)
						{
							if (prev_next == 0)
							{
								prev_next = 1;
								offset = 256;
							}
							else
							{
								prev_next = 0;
								offset = 0;
							}

							cur_off();

							show_page(prev_next);
							print_w1();

							show_sector(Dta,offset);
							print_w2();

							cur_on();

							row_col = 0x1400 | (row_col & 0x00ff);
						}
						else
						{
							row_col = row_col - 0x0100;
							offset--;
						}

						locate(row_col = (row_col & 0xff00) + 77);
						break;

					}

					/*
					 * Characters of ASCII view
					 */

					case 63 :
					case 64 :
					case 65 :
					case 66 :
					case 67 :
					case 68 :
					case 69 :
					case 70 :
					case 71 :
					case 72 :
					case 73 :
					case 74 :
					case 75 :
					case 76 :
					case 77 :
					{
						locate(row_col = (row_col - 0x0001));
						offset--;
						break;
					}

					default :
					{
						break;
					}

				 }
				 break;
			}

			/*
			 * Control Right and Left Arrow
			 */

			case 115 :
			case 116 :
			{

				if (view)
				{
					switch (row_col & 0x00ff)
					{

						case 9 :
						case 10 :
						{
							row_col = (row_col & 0xff00) + 62;
							break;
						}

						case 12 :
						case 13 :
						{
							row_col = (row_col & 0xff00) + 63;
							break;
						}

						case 15 :
						case 16 :
						{
							row_col = (row_col & 0xff00) + 64;
							break;
						}

						case 18 :
						case 19 :
						{
							row_col = (row_col & 0xff00) + 65;
							break;
						}

						case 21 :
						case 22 :
						{
							row_col = (row_col & 0xff00) + 66;
							break;
						}

						case 24 :
						case 25 :
						{
							row_col = (row_col & 0xff00) + 67;
							break;
						}

						case 27 :
						case 28 :
						{
							row_col = (row_col & 0xff00) + 68;
							break;
						}

						case 30 :
						case 31 :
						{
							row_col = (row_col & 0xff00) + 69;
							break;
						}

						case 35 :
						case 36 :
						{
							row_col = (row_col & 0xff00) + 70;
							break;
						}

						case 38 :
						case 39 :
						{
							row_col = (row_col & 0xff00) + 71;
							break;
						}

						case 41 :
						case 42 :
						{
							row_col = (row_col & 0xff00) + 72;
							break;
						}

						case 44 :
						case 45 :
						{
							row_col = (row_col & 0xff00) + 73;
							break;
						}

						case 47 :
						case 48 :
						{
							row_col = (row_col & 0xff00) + 74;
							break;
						}

						case 50 :
						case 51 :
						{
							row_col = (row_col & 0xff00) + 75;
							break;
						}

						case 53 :
						case 54 :
						{
							row_col = (row_col & 0xff00) + 76;
							break;
						}

						case 56 :
						case 57 :
						{
							row_col = (row_col & 0xff00) + 77;
							break;
						}

						default :
						{
							break;
						}

					}

					view = 0;

				}
				else
				{
					switch (row_col & 0x00ff)
					{

						case 62 :
						{
							row_col = (row_col & 0xff00) + 9;
							break;
						}

						case 63 :
						{
							row_col = (row_col & 0xff00) + 12;
							break;
						}

						case 64 :
						{
							row_col = (row_col & 0xff00) + 15;
							break;
						}

						case 65 :
						{
							row_col = (row_col & 0xff00) + 18;
							break;
						}

						case 66 :
						{
							row_col = (row_col & 0xff00) + 21;
							break;
						}

						case 67 :
						{
							row_col = (row_col & 0xff00) + 24;
							break;
						}

						case 68 :
						{
							row_col = (row_col & 0xff00) + 27;
							break;
						}

						case 69 :
						{
							row_col = (row_col & 0xff00) + 30;
							break;
						}

						case 70 :
						{
							row_col = (row_col & 0xff00) + 35;
							break;
						}

						case 71 :
						{
							row_col = (row_col & 0xff00) + 38;
							break;
						}

						case 72 :
						{
							row_col = (row_col & 0xff00) + 41;
							break;
						}

						case 73 :
						{
							row_col = (row_col & 0xff00) + 44;
							break;
						}

						case 74 :
						{
							row_col = (row_col & 0xff00) + 47;
							break;
						}

						case 75 :
						{
							row_col = (row_col & 0xff00) + 50;
							break;
						}

						case 76 :
						{
							row_col = (row_col & 0xff00) + 53;
							break;
						}

						case 77 :
						{
							row_col = (row_col & 0xff00) + 56;
							break;
						}

						default :
						{
							break;
						}

					}

					view = 1;
					hi_lo = 1;
				}			

				locate(row_col);

			}

			/*
			 * Undo Key (-)
			 */

			case 45 :
			{
				break;
			}

			/*
			 * Help Key (+)
			 */

			case 43 :
			{
				break;
			}

			/*
			 * Del Key
			 * Ins Key
			 */

			case 83 :
			case 82 :
			{
				break;
			}

			/*
			 * All other keys
			 */

			default :
			{

				/*
				 * Edit keys
				 *
				 * If HEX view (1 = HEX, 0 = ASCII)
				 */

				if (view)
				{

					/*
					 * Hex field - receive and display character accordingly
					 * (Take only hex characters)
					 */

					switch(key_al)
					{

						/*
						 * 1, 2, 3, 4, 5, 6, 7, 8, 9, 0
						 * A, a, B, b, C, c, D, d, E, e, F, f
						 */

						case 48 :
						case 49 :
						case 50 :
						case 51 :
						case 52 :
						case 53 :
						case 54 :
						case 55 :
						case 56 :
						case 57 :
						case 65 :
						case 66 :
						case 67 :
						case 68 :
						case 69 :
						case 70 :
						case 97 :
						case 98 :
						case 99 :
						case 100 :
						case 101 :
						case 102 :
						{

							/*
							 * In hex field display appropriately
							 *
							 * if upper case convert to lower case
							 */

							if (key_al > 0x39)
							{
								key_al &= 0xdf;
							}

							/*
							 * Save ascii value to print
							 */

							hex[0] = key_al;

							/*
							 * Convert to hex for buffer
							 */

							if(key_al > 0x39)
							{
								key_al -= (char) ( (unsigned int) '7');
							}
							else
							{
								key_al -= (char) ( (unsigned int) '0');
							}

							/*
							 * Place in disk buffer
							 */

							Dta[offset] =
								(
									hi_lo ?
									( (key_al << 4) | (Dta[offset] & 0x0f) ) :
									( (Dta[offset] & 0xf0) | key_al)
								);

							/*
							 * Print character
							 */

							locate(row_col);
							print(hex);

							/*
							 * Locate for ascii
							 * Display ascii
							 * Reset cusror so it can be incremented properly
							 */

							locate((row_col & 0xff00) | ((offset % 16) + 62));
							ascii[0] = Dta[offset];

							/*
							 * If ascii character is the non printable
							 * character 0 or ff then show blank
							 */

							if (ascii[0] == 0 || ascii[0] == 0xff)
							{
								ascii[0] = ' ';
							}

							print(ascii);
							locate(row_col);

							print_row(row_col >> 8);

							/*
							 * Increment cursor (like right arrow)
							 */

							switch(row_col & 0x00ff)
							{

								case 9 :
								case 12 :
								case 15 :
								case 18 :
								case 21 :
								case 24 :
								case 27 :
								case 30 :
								{
									locate(row_col = (row_col + 0x0001));
									hi_lo = 0;
									break;
								}

								case 10 :
								case 13 :
								case 16 :
								case 19 :
								case 22 :
								case 25 :
								case 28 :
								{
									locate(row_col = (row_col + 0x0002));
									offset++;
									hi_lo = 1;
									break;
								}

								case 31 :
								{
									locate(row_col = (row_col & 0xff00) + 35);
									offset++;
									hi_lo = 1;
									break;
								}

								case 35 :
								case 38 :
								case 41 :
								case 44 :
								case 47 :
								case 50 :
								case 53 :
								case 56 :
								{
									locate(row_col = (row_col + 0x0001));
									hi_lo = 0;
									break;
								}

								case 36 :
								case 39 :
								case 42 :
								case 45 :
								case 48 :
								case 51 :
								case 54 :
								{
									locate(row_col = (row_col + 0x0002));
									offset++;
									hi_lo = 1;
									break;
								}

								case 57 :
								{
									if ((row_col & 0xff00) == 0x1400)
									{
										if (prev_next == 1)
										{
											prev_next = 0;
											offset = 0;
										}
										else
										{
											prev_next = 1;
											offset = 256;
										}

										cur_off();

										show_page(prev_next);
										print_w1();

										show_sector(Dta,offset);
										print_w2();

										cur_on();

										row_col = 0x0500 | (row_col & 0x00ff);
									}
									else
									{
										row_col = row_col + 0x0100;
										offset++;
									}

									hi_lo = 1;

									locate(row_col = (row_col & 0xff00) + 9);
									break;
								}

								default :
								{
									break;
								}

							}

  							break;

						}

						/*
						 * Any other key
						 */

						default :
						{
							break;
						}

					}
					
				}
				else
				{

					/*
					 * In ascii field - receive and display accordingly
					 *	(Take any key that has not been filtered)
					 * Convert to hex for buffer
					 */

					show_hex(key_al, hex);

					/*
					 * Place in disk buffer
					 */

					Dta[offset] = key_al;

					/*
					 * Set cursor and print hex characters
					 */

					if ((offset % 16) < 8)
					{
						locate((row_col & 0xff00) + 9 + ((offset % 8) * 3));
					}
					else
					{
						locate((row_col & 0xff00) + 35 + ((offset % 8) * 3));
					}

					print(hex);

					/*
					 * Reset buffer to display 1 character (used by hex)
					 */

					hex[1] = 0;

					/*
					 * Display ascii
					 * Reset cusror so ti can be incremented properly
					 */

					ascii[0] = Dta[offset];

					/*
					 * If ascii character is the non printable
					 * character 0 or ff then show blank
					 */

					if (ascii[0] == 0 || ascii[0] == 0xff)
					{
						ascii[0] = ' ';
					}

					locate(row_col);
					print(ascii);
					locate(row_col);

					print_row(row_col >> 8);

					/*
					 * Increment cursor (like right arrow)
					 */

					switch(row_col & 0x00ff)
					{

						case 62 :
						case 63 :
						case 64 :
						case 65 :
						case 66 :
						case 67 :
						case 68 :
						case 69 :
						case 70 :
						case 71 :
						case 72 :
						case 73 :
						case 74 :
						case 75 :
						case 76 :
						{
							locate(row_col = (row_col + 0x0001));
							offset++;
							break;
						}

						case 77 :
						{
							if ((row_col & 0xff00) == 0x1400)
							{
								if (prev_next == 1)
								{
									prev_next = 0;
									offset = 0;
								}
								else
								{
									prev_next = 1;
									offset = 256;
								}

								cur_off();

								show_page(prev_next);
								print_w1();

								show_sector(Dta,offset);
								print_w2();

								cur_on();

								row_col = 0x0500 | (row_col & 0x00ff);
							}
							else
							{
								row_col = row_col + 0x0100;
								offset++;
							}

							locate(row_col = (row_col & 0xff00) + 62);
							break;
						}

						default :
						{
							break;
						}

					}

				}

				break;
			}

		}

	}
	while(next_key == 01);

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ					 	select_disk(drive)				  	  		  ณ
 ณ																	  ณ
 ณ				  Select head track and sector on disk				  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

select_disk(drive)

unsigned int *drive;

{

	struct position					 /* Select sector structure			   */
	{
		unsigned int sts_select;	 /* Changing head track or sector	   */
		unsigned int sts_pos[1];	 /* Row Column on screen			   */
		unsigned int min_max[2];	 /* Mimimum Maximum value for sts	   */
		unsigned int sts_value[1];	 /* Current sts values				   */
	};

	struct position disk;		/* Select sector structure				   */
	unsigned int key;			/* Scan code of pressed key				   */
	char *show_hex();			/* Convert to hex						   */
	unsigned int current;		/* Current selection					   */
	unsigned int next;			/* Next selection						   */

	/*
	 * Initialize select disk sector structure
	 */

	disk.sts_select = 0;			  /* Selecting head, track, sector or drive   */

	disk.sts_pos[0] = 0x1608;		  /* Row Col of Drive				   */

	read_boot_record(*drive);

	if (Max_track == 0 || Max_sector == 0)
	{
		check_disk(1,*drive);
		return(1);
	}

	disk.min_max[0] = 0;			  /* Drive minimum value = 0		   */
	disk.min_max[1] = Max_drive;	  /* Drive maximum value = Max_drive   */

	disk.sts_value[0] = *drive;		  /* Current Drive value				   */

	/*
	 * Drive selection picked by default
	 */

	show_select_ascii(1,disk.sts_pos[0],disk.sts_value[0]);
	print_w3();

	do
	{

		key = get_key() >> 8;

		switch(key)
		{

			/*
			 * Previous key
			 */

			case 0x0049 :
			case 0x0084 :
			{
				current = disk.sts_select;

				if(disk.sts_value[current] == disk.min_max[2*current])
				{
					disk.sts_value[current] = disk.min_max[(2*current)+1];
				}
				else
				{
					disk.sts_value[current] -= 1;
				}

				/*
			 	 * Determine if ascii value or hex value is displayed
				 */

				if (current == 3)
				{
					show_select_ascii(1,disk.sts_pos[current],disk.sts_value[current]);
				}
				else
				{
					show_select_ascii(1,disk.sts_pos[current],disk.sts_value[current]);
				}	

				print_row(0x16);

				break;
			}

			/*
			 * Next Key
			 */

			case 0x0051 :
			case 0x0076 :
			{
				current = disk.sts_select;

				if(disk.sts_value[current] == disk.min_max[(2*current)+1])
				{
					disk.sts_value[current] = disk.min_max[2*current];
				}
				else
				{
					disk.sts_value[current] += 1;
				}

				/*
			 	 * Determine if ascii value or hex value is displayed
				 */

				if (current == 3)
				{
					show_select_ascii(1,disk.sts_pos[current],disk.sts_value[current]);
				}
				else
				{
					show_select_ascii(1,disk.sts_pos[current],disk.sts_value[current]);
				}

				print_row(0x16);

				break;
			}

			/*
			 * All other keys
			 */

			default :
			{
				break;
			}

		}

	}
	while(key != 01);

	/*
	 * Ouptut Disk Location
	 */

	print_trailer("Drive  ","Reading Boot Record");
	print_menu("Select","Read","Write"," "," ");
	show_select_ascii(0,0x1608,disk.sts_value[0]);
	print_w3();

	read_boot_record(disk.sts_value[0]);

	if (Max_sector == 0 || Max_track == 0)
	{
		check_disk(1,disk.sts_value[3]);
		return(1);
	}

	print_trailer("Drive  ","ESC to ACCEPT");
	print_menu("Select","Read","Write"," "," ");
	show_select_ascii(0,0x1608,disk.sts_value[0]);
	print_w3();
			
	/*
	 * Set head track sector
	 */

	*drive = disk.sts_value[0];		/* Current drive value	*/

	return(0);

}

