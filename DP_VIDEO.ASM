;
; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ								       บ
; บ (C)opyright 1987, Personal Touch Software Inc. All Rights Reserved บ
; บ								       บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
;

title    dp_video

;
; *** Include
;

include sm8086.mac

_TEXT	segment	byte public 'CODE'
_TEXT	ends
_DATA	segment word public 'DATA'
_DATA	ends
CONST	segment word public 'CONST'
CONST	ends
_BSS	segment word public 'BSS'
_BSS	ends
DGROUP	GROUP	CONST,	_BSS,	_DATA
        assume CS:_TEXT,DS:DGROUP,SS:DGROUP,ES:DGROUP

;
; *** Publics
;

public _PEEK
public _POKE
public _SHOW_VID

Page

;
; 
; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ                              	                               บ
; บ			       EQUATES  			       บ
; บ								       บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

BLOCK_SIZE                   equ       0f0h

Page

_TEXT			segment byte public 'CODE'

;
; ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
; บ								       บ
; บ                          DEFINED STORAGE                           บ
; บ								       บ
; ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
;

bmove                        db        BLOCK_SIZE

Page

;
; ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
; ณ								       ณ
; ณ peek(segment, offset, buffer_address, number_of_bytes);            ณ
; ณ								       ณ
; ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
;

_PEEK	 proc      near

         push      bp
         mov       bp,sp
         push      cx
         push      si
         push      di
         push      ds
         push      es
         pushf

         cld

;
; *** Set up destination segment (same as current _DATA segment)
;

         push      ds
         pop       es

;
; *** Get source segment
;

         mov       ds,4[bp]            ; get source segment

;
; *** Get source offset
;

         mov       si,6[bp]            ; get source offset


         mov       di,8[bp]           ; get destination offset

;
; *** Get the number of characters (words) to move
;

         mov       cx,10[bp]

;
; *** Perform block move
;

         cld

         rep       movsb

;
; *** Clean up and return
;

         popf
         pop       es
         pop       ds
         pop       di
         pop       si
         pop       cx
         pop       bp

         ret

_PEEK     endp

Page

;
; ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
; ณ								       ณ
; ณ poke(segment, offset, buffer_address, number_of_bytes);            ณ
; ณ								       ณ
; ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
;

_POKE	 proc      near

         push      bp
         mov       bp,sp
         push      cx
         push      si
         push      di
         push      es
         pushf

         cld

;
; *** Get source segment
;

         mov       es,4[bp]            ; get destination segment

;
; *** Get source offset
;

         mov       di,6[bp]            ; get destination offset


         mov       si,8[bp]           ; get source offset

;
; *** Get the number of characters (words) to move
;

         mov       cx,10[bp]

;
; *** Perform block move
;

         cld

         rep       movsb

;
; *** Clean up and return
;

         popf
         pop       es
         pop       di
         pop       si
         pop       cx
         pop       bp

         ret

_POKE    endp

;
; ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
; ณ								       ณ
; ณshow_video(source_seg,source_off,dest_seg,dest_off,no_of_characters)ณ
; ณ								       ณ
; ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
;

find_me  dw        3412h

_SHOW_VID proc     near

         push      bp
         mov       bp,sp
         push      ax
         push      bx
         push      cx
         push      dx
         push      si
         push      di
         push      ds
         push      es
         pushf

         cld

;
; *** Get source segment
;

         mov       ds,4[bp]            ; get source segment

;
; *** Get source offset
;

         mov       si,6[bp]            ; get source offset

;
; *** Get destination segment
;

         mov       es,8[bp]            ; get destination segment

;
; *** Get destination offset
;

         mov       di,10[bp]            ; get destination offset

;
; *** Get the number of characters (words ) to move
;

         mov       cx,12[bp]

;
; *** Calculate move to video buffer
; *** CX = number of characters to move / three rows of characters (240)
; *** AX = number of characters to move % three rows of characters (240)
;

        mov        ax,cx
        div        bmove

        xor        ch,ch
        mov        cl,al

        mov        al,ah
        xor        ah,ah

        or         cx,cx
        jz         less_than_block

;
; *** Loop to display to screen in 3 row increments
;

page_loop:

         push      cx

         mov       cx,BLOCK_SIZE

         call      color_off

         cld

         rep       movsw

         call      color_on

         pop       cx

         loop      page_loop

;
; *** Display remainder of bytes to video
;

less_than_block:

         mov       cx,ax

         call      color_off

         cld

         rep       movsw

         call      color_on

;
; *** Clean up and return
;

         popf
         pop       es
         pop       ds
         pop       di
         pop       si
         pop       dx
         pop       cx
         pop       bx
         pop       ax
         pop       bp

         ret

_SHOW_VID endp

;
; ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
; ณ 								       ณ
; ณ                           COLOR OFF                                ณ
; ณ 								       ณ
; ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ 
;

color_off proc     near

         push      ax
         push      dx

         mov       dx,3dah

color_off_loop:

         in        al,dx

         test      al,8
         jz        color_off_loop

         mov       al,00001001b
         and       al,0f7h

         mov       dx,03d8h
         out       dx,al

         pop       dx
         pop       ax

         ret

color_off endp

Page

;
; ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ
; ณ								       ณ
; ณ                              COLOR ON                              ณ
; ณ								       ณ
; ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ
;

color_on proc      near

         push      ax
         push      dx

         mov       al,000001001b
         mov       dx,03d8h
         out       dx,al

         pop       dx
         pop       ax

         ret

color_on endp

_TEXT	 ends

         end

