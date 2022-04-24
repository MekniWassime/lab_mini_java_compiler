%{	
 
#include <stdio.h>	
#include <stdlib.h>	          
#include <string.h>
#include <math.h>	
#include "java.tab.h"	

%}

%option yylineno
delim	[ \t]
bl	{delim}+
chiffre	[0-9]
lettre	[a-zA-Z]
id	({lettre}|_|$)({lettre}|{chiffre}|_|$)*
iderrone	{chiffre}({lettre}|{chiffre}|_|$)*
open_parentheses	(\()
closed_parentheses	(\))
open_bracket	(\{)
closed_bracket	(\})
type	(int|int\[\]|boolean)
integer	-?{chiffre}+
boolean	(true|false)
break_line  \n
COMMENT_BLOCK	\/\*(.|\n)*\*\/
COMMENT_BLOCK_ERROR	\/\*([^(\*\/)]|\n)*
COMMENT_LINE	"//"
string  \"[^\"]*\"

%%

{bl}	/* pas d'actions */
"\\n"					++yylineno;
{string}    			return StringLiteral;
"["						return OpenBracket;
"]" 					return ClosedBracket;
{type}					return Type;
{open_parentheses}		return OpenParentheses;
{closed_parentheses} 	return ClosedParentheses;
{open_bracket}			return OpenCurlyBracket;
{closed_bracket}		return ClosedCurlyBracket;
"!="					return InequalityOperator; 
"=="					return EqualityOperator; 
"<="					return LessOrEqualOperator;
"<"						return LessOperator;
">="					return GreaterOrEqualOperator;
">"						return GreaterOperator;
"-"						return SubstractOperator; 
"*"						return MultiplicationOperator;
"="						return AssignmentOperator;
"/"						return DivisionOperator;
"+"						return AdditionOperator;
"&&"					return BooleanAndOperator;
"||"					return BooleanOrOperator;
","						return Comma;
"."						return Point;
";" 					return SemiColon;
"!"						return NegationOperator;
{integer}				return IntegerLiteral;
{boolean}				return BooleanLiteral;
"class"					return ClassKeyword;
"public"				return PublicKeyword;
"static"				return StaticKeyword;
"extends"				return ExtendsKeyword;
"private"				return PrivateKeyword;
"package"				return PackageKeyword;
"main"					return MainKeyword;
"String"				return StringKeyword;
"if"					return IfStatement;
"else"					return ElseStatement;
"while"					return WhileStatement;
"System.out.println"	return PrintLnKeyword;
"length"				return LengthGetter;
"this"					return ThisKeyword;
"new"					return NewKeyword;
"return" 				return ReturnKeyword;
"void"					return VoidKeyword;
{COMMENT_LINE}			{}
{COMMENT_BLOCK}			{}
{id}					return Identifier;

{iderrone}	{fprintf(stderr,"illegal identifier \'%s\' on line :%d",yytext,yylineno);}
{COMMENT_BLOCK_ERROR}	{fprintf(stderr,"block comment not closed on line :%d",yylineno);}
	

%%

int yywrap()
{
	return(1);
}
