/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKHEAT = 258,
     TOKTARGET = 259,
     TOKTEMPERATURE = 260,
     SEND = 261,
     PACKET = 262,
     HELP = 263,
     EXIT = 264,
     EOS = 265,
     ALL = 266,
     DATA_PLAN_RESET = 267,
     ADD = 268,
     DEL = 269,
     SHOW = 270,
     SET = 271,
     AC = 272,
     WTP = 273,
     STA = 274,
     IP = 275,
     MAC = 276,
     TYPE = 277,
     CTR_PORT = 278,
     DATA_PORT = 279,
     MINI_JUMBO = 280,
     INDEX = 281,
     VLAN_ID = 282,
     RID = 283,
     ACL = 284,
     RUNTIME = 285,
     PORT = 286,
     CPU = 287,
     CAPWAP = 288,
     INFO = 289,
     STATE = 290,
     _INT_ = 291,
     _IPADDR_ = 292,
     _MACADDR_ = 293
   };
#endif
/* Tokens.  */
#define TOKHEAT 258
#define TOKTARGET 259
#define TOKTEMPERATURE 260
#define SEND 261
#define PACKET 262
#define HELP 263
#define EXIT 264
#define EOS 265
#define ALL 266
#define DATA_PLAN_RESET 267
#define ADD 268
#define DEL 269
#define SHOW 270
#define SET 271
#define AC 272
#define WTP 273
#define STA 274
#define IP 275
#define MAC 276
#define TYPE 277
#define CTR_PORT 278
#define DATA_PORT 279
#define MINI_JUMBO 280
#define INDEX 281
#define VLAN_ID 282
#define RID 283
#define ACL 284
#define RUNTIME 285
#define PORT 286
#define CPU 287
#define CAPWAP 288
#define INFO 289
#define STATE 290
#define _INT_ 291
#define _IPADDR_ 292
#define _MACADDR_ 293




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 65 "the.y"
{
	char *sptr;
	void *vptr;
	unsigned int ipaddr;
	unsigned int ival;
	unsigned char macaddr[6];
}
/* Line 1529 of yacc.c.  */
#line 133 "the.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

