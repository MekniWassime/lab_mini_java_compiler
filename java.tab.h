
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
     StringLiteral = 258,
     OpenBracket = 259,
     ClosedBracket = 260,
     Type = 261,
     OpenParentheses = 262,
     ClosedParentheses = 263,
     OpenCurlyBracket = 264,
     ClosedCurlyBracket = 265,
     Identifier = 266,
     InequalityOperator = 267,
     EqualityOperator = 268,
     LessOrEqualOperator = 269,
     LessOperator = 270,
     GreaterOrEqualOperator = 271,
     GreaterOperator = 272,
     SubstractOperator = 273,
     MultiplicationOperator = 274,
     AssignmentOperator = 275,
     DivisionOperator = 276,
     AdditionOperator = 277,
     BooleanAndOperator = 278,
     BooleanOrOperator = 279,
     PackageKeyword = 280,
     Comma = 281,
     Point = 282,
     SemiColon = 283,
     IntegerLiteral = 284,
     BooleanLiteral = 285,
     ClassKeyword = 286,
     PublicKeyword = 287,
     StaticKeyword = 288,
     ExtendsKeyword = 289,
     PrivateKeyword = 290,
     MainKeyword = 291,
     StringKeyword = 292,
     IfStatement = 293,
     ElseStatement = 294,
     WhileStatement = 295,
     PrintLnKeyword = 296,
     LengthGetter = 297,
     ThisKeyword = 298,
     NewKeyword = 299,
     ReturnKeyword = 300,
     LineComment = 301,
     BlockComment = 302,
     VoidKeyword = 303,
     NegationOperator = 304
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


