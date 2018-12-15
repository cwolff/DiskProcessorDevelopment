/*	Last revised : Wed 9-Sep-87 at 3:05 pm*/
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

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 					  show_sector(cdta,offset)				 		  ณ
 ณ																	  ณ
 ณ 			  Show sector in current dta at given offet		 		  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

show_sector(cdta,offset)

char *cdta;				 /* Current Disk Transfer Address			*/
unsigned int offset;				 /* Offset into cdta to show				  */

{

	unsigned int loop1;		/* Loop counter first layer				  */
	unsigned int loop2;		/* Loop counter second layer			  */
	char hex[3];			/* Hex data string						  */
	char ascii[2];			/* Ascii data							  */

	/*
	 * Initialize end of hex string
	 */

	hex[2] = '\0';
	ascii[1] ='\0';

	/*
	 * Output disk data header
	 */

	color(Txt_attr | F_BRIGHT);

	locate(0x0300);

	print(" OFF                         Hex View                            Ascii View");

	/*
	 * Calculate offset into cdta
	 */

	cdta += offset;

	color(Txt_attr);

	locate(0x0500);

	/*
	 * Display disk data in hex by 16 rows of 16
	 * along with the ascii data
	 */

	for(loop1=0;loop1<16;loop1++)
	{

		/*
		 * Display Side, track, sector and disk offset from start of sector
		 */

		if(offset == 0)
		{
			if(loop1 == 0)
			{
				print(" 000     ");
			}
			else
			{
				print(" 0");
				show_hex(loop1 * 0x0010,hex);
				print(hex);
				print("     ");
			}
		}
		else
		{
			if(loop1 == 0)
			{
				print(" 100     ");
			}
			else
			{
				print(" 1");
				show_hex(loop1*0x0010,hex);
				print(hex);
				print("     ");
			}
		}

		/*
		 * Display the first group of 8 values
		 */

		for(loop2=0;loop2<8;loop2++)
		{
			show_hex(*(cdta + (loop1 * 16) + loop2),hex);
			print(hex);
			print(" ");
		}

		/*
		 * Display the group seperator
		 */

		print("- ");

		/*
		 * Display the second group of 8 values
		 */

		for(loop2=8;loop2<16;loop2++)
		{
			show_hex(*(cdta + (loop1 * 16) + loop2),hex);
			print(hex);
			print(" ");
		}

		/*
		 * Display space seperation between hex and ascii view of data
		 */

		print("   ");

		/*
		 * Display the same 16 bytes of data in ascii
		 */

		for(loop2=0;loop2<16;loop2++)
		{
			ascii[0] = *(cdta + (loop1*16)+loop2);

			/*
			 * If non printable ascii character 0 or ff then show blank
			 */

			if (ascii[0] == 0 || ascii[0] == 0xff)
			{
				ascii[0] = ' ';
			}

			print(ascii);

		}

		/*
		 * Data line completed display a new line
		 */

		locate(0x0600 + (loop1 << 8));

	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 			    show_select_hex(switch,position,value)		 		  ณ
 ณ																	  ณ
 ณ 			  	  	show selection as on or off			 		  	  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

show_select_hex(on_off,position,value)

unsigned int on_off;				  /* Choice is on or off			*/
unsigned int position;				  /* Value position on screen		*/
unsigned int value;					  /* Current value to display		*/

{

	char strng[6];				   	 /* Value string					*/

	/*
	 * Initialize value string
	 */

	strng[4] = '\0';
	strng[5] = '\0';

	if(on_off == 0)
	{

		color(Tail_txt_attr);

		strng[0] = ' ';

		if (value < 256)
		{
			strng[3] = ' ';
		}
		else
		{
			strng[4] = ' ';
		}

	}
	else
	{

		color(Tail_txt_attr << 4);

		strng[0] = (char) 0x00dd;
		if (value < 256)
		{
			strng[3] =  (char) 0x00de;
			strng[4] =  (char) 0x00db;		/* When reverse video goes from 3	*/
											/* digits to 2 digits when need to	*/
											/* have a solid block to blank		*/
											/* screen							*/
		}
		else
		{
			strng[4] =  (char) 0x00de;
		}

	}

	locate(position);

	show_sector_hex(value,&strng[1]);

	print(strng);

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ 			    show_select_ascii(switch,position,value)	 		  ณ
 ณ																	  ณ
 ณ 			  	  	show selection as on or off			 		  	  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

show_select_ascii(on_off,position,value)

unsigned int on_off;				  /* Choice is on or off			*/
unsigned int position;				  /* Value position on screen		*/
unsigned int value;					  /* Current value to display		*/

{

	char strng[4];				   	 /* Value string					*/

	strng[3] = '\0';

	if(on_off == 0)
	{

		color(Tail_txt_attr);

		strng[0] = ' ';
		strng[2] = ' ';

	}
	else
	{

		color(Tail_txt_attr << 4);

		strng[0] = (char) 0x00dd;
		strng[2] =  (char) 0x00de;

	}

	locate(position);

	strng[1] = (char) ((value + (int) 'A') & 0x00ff);

	print(strng);

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ						  show_page(page_no)						  ณ
 ณ																	  ณ
 ณ						  show page 0 or page 1						  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

show_page(page_no)

unsigned int page_no;					/* prev or next page					  */

{

	/*
	 * Initialize value string
	 */

	color(Hdr_attr);

	locate(0x0040);

	if(page_no == 0)
	{
		print("[ Page 1 of 2 ]");
	}
	else
	{
		print("[ Page 2 of 2 ]");
	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ			 unsigned char show_hex(character,hex_ptr)				  ณ
 ณ																	  ณ
 ณ		  Convert one character recieved in character to hex		  ณ
 ณ		  and place into a hex string pointed to by hex_ptr			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

show_hex(character,hex_ptr)

unsigned char character;	  /* Character to convert							 */
unsigned char *hex_ptr;		/* Pointer to the converted hex string		 */

{

	char right_digit;
	char left_digit;

	/*
	 * Each Character will have two hex digits.
	 * First get the high byte and place in the hex string
	 * Then get the low byte and place in the hex string
	 */

	if((left_digit = ((unsigned int) (character >> 4))) >= 0x0a)
	{
		*hex_ptr++ = (char) ( (unsigned int) '7' + left_digit);
	}
	else
	{
		*hex_ptr++ = (char) ( (unsigned int) '0' + left_digit);
	}

	if((right_digit = ((unsigned int) (character & 0x0f))) >= 0x0a)
	{
		*hex_ptr = (char) ( (unsigned int) '7' + right_digit);
	}
	else
	{
		*hex_ptr = (char) ( (unsigned int) '0' + right_digit);
	}

}

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ			 unsigned char show_sector_hex(character,hex_ptr)		  ณ
 ณ																	  ณ
 ณ		  Convert one integer recieved in character to hex			  ณ
 ณ		  and place into a hex string pointed to by hex_ptr			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

show_sector_hex(hex_number,hex_ptr)

unsigned int hex_number;	/* Character to convert				 	 	*/
unsigned char *hex_ptr;		/* Pointer to the converted hex string		*/

{

	/*
	 * number 321, 1 is the 1st digit, 2 is the second digit, 3 is the third digit
	 */

	unsigned char first_digit;
	unsigned char second_digit;
	unsigned char third_digit;

	/*
	 * Each Character will have two hex digits.
	 * First get the high byte and place in the hex string
	 * Then get the low byte and place in the hex string
	 */

	if (hex_number > 255)
	{
		if((third_digit = ( ((hex_number >> 8) & 0x0f) )) >= 0x0a)
		{
			*hex_ptr++ = (char) ( (unsigned int) '7' + third_digit);
		}
		else
		{
			*hex_ptr++ = (char) ( (unsigned int) '0' + third_digit);
		}
	}

	if((second_digit = ( ((hex_number >> 4)  & 0x0f) )) >= 0x0a)
	{
		*hex_ptr++ = (char) ( (unsigned int) '7' + second_digit);
	}
	else
	{
		*hex_ptr++ = (char) ( (unsigned int) '0' + second_digit);
	}

	if((first_digit = ((hex_number & 0x0f))) >= 0x0a)
	{
		*hex_ptr = (char) ( (unsigned int) '7' + first_digit);
	}
	else
	{
		*hex_ptr = (char) ( (unsigned int) '0' + first_digit);
	}

}
