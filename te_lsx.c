/*	te_lsx.c

	Text editor -- version for the LSX-Dodgers.

	Copyright (c) 2015-2021 Miguel Garcia / FloppySoftware

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the
	Free Software Foundation; either version 2, or (at your option) any
	later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

	Usage:

	te_lsx [filename]

	Compilation:

	cc te_lsx
	ccopt te_lsx
	zsm te_lsx
	hextocom te_lsx

	Changes:

	08 Aug 2022 : 1st version from source for the Amstrad CPC with CP/M 3.

	Notes:

	LSX-Dodgers is an operating system for running CP/M80 and MSX-DOS software on SHARP X1/turbo/Z.
	It has MS-DOS compatible file formats and system calls compatible with CP/M80 and MSX-DOS.

	https://github.com/tablacus/LSX-Dodgers

*/

/* Default configuration values
   ----------------------------
*/
#define CRT_DEF_ROWS 24
#define CRT_DEF_COLS 80

/* Options
   -------
   Set to 1 to add the following functionalities, else 0.
*/
#define OPT_LWORD 0  /* Go to word on the left */
#define OPT_RWORD 0  /* Go to word on the right */
#define OPT_FIND  1  /* Find string */
#define OPT_GOTO  1  /* Go to line # */
#define OPT_BLOCK 1  /* Block selection */
#define OPT_MACRO 1  /* Enable macros */

/* Include main code
   -----------------
*/
#include "te.c"

/* Setup CRT: Used when the editor starts
   --------------------------------------
   void CrtSetup(void)
*/
CrtSetup()
{
	CrtSetupEx();
	
	CrtOut(27); CrtOut('j'); /* clear screen */
}

#asm
CrtSetupEx:
	ret

BiosConst:
	ld	c,0x0b
	jp	5

BiosConin:  
	ld	c,7
	jp	5

BiosConout: 
	ld	e,c
	ld	c,2
	jp	5

#endasm

/* Reset CRT: Used when the editor exits
   -------------------------------------
   void CrtReset(void)
*/
CrtReset()
{
	CrtOut(27); CrtOut('j'); /* clear screen */
}

/* Output character to the CRT
   ---------------------------
   All program output is done with this function.
   
   On '\n' outputs '\n' + '\r'.

   void CrtOut(int ch)
*/
#asm
CrtOut:
	ld   c,l
	ld   a,l
	cp   10
	jp   nz,BiosConout
	call BiosConout
	ld   c,13
	jp   BiosConout
#endasm

/* Input character from the keyboard
   ---------------------------------
   All program input is done with this function.

   int CrtIn(void)
*/
#asm
CrtIn:
	call BiosConin
	ld h,0
	ld l,a
	ret
#endasm

/* Clear screen and send cursor to 0,0
   -----------------------------------
   void CrtClear(void)
*/
CrtClear()
{
	CrtOut(27); CrtOut('j'); /* clear screen */
	CrtOut(27); CrtOut('H'); /* Cursor to 0,0 */
}

/* Locate the cursor (HOME is 0,0)
   -------------------------------
   void CrtLocate(int row, int col)
*/
CrtLocate(row, col)
int row, col;
{
	CrtOut(27); CrtOut('Y'); CrtOut(row + 32); CrtOut(col + 32);
}

/* Erase line and cursor to row,0
   ------------------------------
   void CrtClearLine(int row)
*/
CrtClearLine(row)
int row;
{
	CrtLocate(row, 0); CrtClearEol();
}

/* Erase from the cursor to the end of the line
   --------------------------------------------
*/
CrtClearEol()
{
	CrtOut(27); CrtOut('K');
}

/* Turn on / off reverse video
   ---------------------------
*/
CrtReverse(on)
int on;
{
}

