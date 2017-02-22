/***
 *** C# parser/scanner
 *** Copyright 2002 James Power, NUI Maynooth, Ireland <james.power@may.ie>
 *** This version: 19 Feb 2002
 ***
 *** This program is free software; you can redistribute it and/or modify
 *** it under the terms of the GNU General Public License as published by
 *** the Free Software Foundation; either version 2 of the License, or
 *** (at your option) any later version.
 ***
 *** This program is distributed in the hope that it will be useful,
 *** but WITHOUT ANY WARRANTY; without even the implied warranty of
 *** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *** GNU General Public License for more details.
 ***
 *** You should have received a copy of the GNU General Public License
 *** along with this program; if not, write to the Free Software
 *** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ***/

void lex_enter_attrib(void);
void lex_exit_attrib(void); 
void lex_enter_accessor(void);
void lex_exit_accessor(void);
void lex_enter_getset(void);
void lex_exit_getset(void);
 
