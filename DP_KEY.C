/*
 嬪様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様邑
 �																	  �
 �    Disk Processor (C)opyright 1987 Personal Touch Software Inc.    �
 �																	  �
 塒様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様余
*/

/*
 * Include files
 */

#include "dp.h"
#include "dp_extrn.h"
#include "dos.h"

/*
 嬪様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様邑
 �																	  �
 �                           get_key()								  �
 �																	  �
 �                       Get the next key							  �
 �																	  �
 �                Returns : scan code and ascii character			  �
 �																	  �
 塒様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様余
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