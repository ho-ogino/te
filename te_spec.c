/*	te_spec.c

	Text editor -- version for the Spectrum +3.

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

	te_spec [filename]

	Compilation:

	cc te_spec
	ccopt te_spec
	zsm te_spec
	hextocom te_spec

	Changes:

	06 May 2015 : 1st version.
	31 Aug 2015 : Minor changes in comments.
	02 Jun 2016 : Minor changes.
	25 Jan 2018 : Find & find next keys.
	20 Feb 2018 : Macro & go to line # keys.
	30 Dec 2018 : Refactorized i/o functions.
	17 Jan 2019 : Added CrtReverse().
	18 Jan 2019 : Added K_DELETE.
	23 Jan 2019 : Modified a lot for key bindings support.
	29 Jan 2019 : Added K_CLRCLP.
	24 Dec 2019 : Added OPT_NUM.
	26 Dec 2019 : Now K_INTRO is K_CR. Remove CRT_ESC_KEY.
	14 Jan 2021 : Remove OPT_NUM.
	22 Feb 2021 : Move CRT_ROWS, CRT_COLS to assembler.
	04 Apr 2021 : Remove key bindings.
	11 May 2021 : Remove CRT configuration values.
	30 Jun 2021 : Added CRT_DEF_ROWS, CRT_DEF_COLS.

	Notes:

	The Spectrum +3 runs CP/M Plus (v3.1) with a VT52-like emulation,
	and a 24x51 CRT minus 1 line for system messages (it can be enabled
	or disabled).

	It supports 24x32 and 24x80 (in two pages) CRT modes also.
*/

/* Default configuration values
   ----------------------------
*/
#define CRT_DEF_ROWS 23
#define CRT_DEF_COLS 51

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
	
	CrtOut(27); CrtOut('3'); CrtOut(1); /* Set 24 x 51 mode */	
}

#asm
CrtSetupEx:
	ld  hl,(1)
	inc hl
	inc hl
	inc hl
	ld  de,BiosConst
	ld  bc,9
	ldir
	ret

BiosConst:  jp 0
BiosConin:  jp 0
BiosConout: jp 0
#endasm

/* Reset CRT: Used when the editor exits
   -------------------------------------
   void CrtReset(void)
*/
CrtReset()
{
}

/* Output character to the CRT
   ---------------------------
   All program output is done with this function.
   
   On '\n' outputs '\n' + '\r'.

   void CrtOut(int ch)
*/
#asm
CrtOut:
	ld   a,l
	cp   10
	jr   nz,CrtOutRaw
	ld   c,13
	call BiosConout
	ld   l,10
CrtOutRaw:
	ld   c,l
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
	CrtOut(27); CrtOut('H'); /* Cursor to 0,0 */
	CrtOut(27); CrtOut('E'); /* Clear CRT */
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
	CrtOut(27); CrtOut(on ? 'p' : 'q');
}

