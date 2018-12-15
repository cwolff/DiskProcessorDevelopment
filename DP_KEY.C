/*
 ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
 ณ																	  ณ
 ณ    Disk Processor (C)opyright 1987 Personal Touch Software Inc.    ณ
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
 ณ                           get_key()								  ณ
 ณ																	  ณ
 ณ                       Get the next key							  ณ
 ณ																	  ณ
 ณ                Returns : scan code and ascii character			  ณ
 ณ																	  ณ
 ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
*/

int
get_key()

{

   union REGS inregs;      /* Used for interrupt registers           */
   union REGS outregs;

   /*
    * Get next key
    */

   inregs.x.ax = 0x0000;

   int86(0x0016, &inregs, &outregs);

   return(outregs.x.ax);

}

