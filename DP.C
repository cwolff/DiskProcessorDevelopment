/*	Last revised : Sun 13-Sep-87 at 1:15 pm*/
/*
 ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
 บ	   		   	   tm												  บ
 บ	Disk Processor   (C)opyright 1987 Personal Touch Software Inc.    บ
 บ																	  บ
 ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

/*
 * Include files
 */

#include "dp.h"
#include "dp_extrn.h"

/*
 * External functions
 */

extern int	reset();

/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ							   main()								  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

void

main()

{

	unsigned short int key;	  		/* Scan code of pressed key	  	*/
	unsigned short int mode;		/* Mode to open the disk		*/
	unsigned short view;			/* Current view of the disk		*/

	/*
	 * Hardware and Video Initialization Routine to set up Disk Processor
	 */

	init();

	cur_off();

	color(B_BLACK | F_WHITE);
	clr_scrn();

	view = SECTOR;

	print_first_menu(view);

	do
	{

		key = get_key() >> 8;

		switch(key)
		{

			/*
			 * Select View
			 */

			case 0x003b :
			case 0x0054 :
			case 0x005e :
			case 0x0068 :
			{

				select_view(&view);

				/*
				 * Show menu again
				 */

				cur_off();

				color(B_BLACK | F_WHITE);
				clr_scrn();

				print_first_menu(view);

				break;
			}

			/*
			 * How to use Disk Processor
			 */

			case 0x003c :
			case 0x0055 :
			case 0x005f :
			case 0x0069 :
			{
				display_error(10,"How to use Disk Processor is under construction.");
				break;
			}

			/*
			 * Open Disk
			 */

			case 0x003d :
			case 0x0056 :
			case 0x0060 :
			case 0x006a :
			{

				/*
				 * open disk
				 */

				switch(view)
				{

					case SECTOR	:
					{
						sector_view();

						/*
						 * Show menu again
						 */
	
						cur_off();
	
						color(B_BLACK | F_WHITE);
						clr_scrn();
		
						print_first_menu(view);

					   	break;
					}
					
					case BOOT :
					{

						boot_record_view();
						
						/*
						 * Show menu again
						 */
	
						cur_off();
	
						color(B_BLACK | F_WHITE);
						clr_scrn();
		
						print_first_menu(view);

					   	break;
					}
					
					case FAT :
					{
						display_error(15,"The File Allocation Table View is under construction.");

						break;
					}
					
					case DIR :
					{
						display_error(15,"The Directory View is under construction.");

						break;
					}

					case FILE :
					{
						display_error(15,"The File View is under construction.");

						break;
					}

				}

				break;
			}

			/*
			 * All other Keys
			 */

			default :
			{
				break;
			}

		}

	}
	while(key != 01);

	/*
	 * Clear screen and exit
	 */

	locate(0x0000);
	color(B_BLACK | F_WHITE);
	clr_scrn();
	print_scrn();

	reset();

}

