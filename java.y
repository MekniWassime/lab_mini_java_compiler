%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno;

int yyerror(char const * msg);	
int yylex(void);

%}
%error-verbose
%token StringLiteral 
%token OpenBracket
%token ClosedBracket
%token Type
%token OpenParentheses
%token ClosedParentheses
%token OpenCurlyBracket
%token ClosedCurlyBracket
%token Identifier
%token InequalityOperator
%token EqualityOperator
%token LessOrEqualOperator
%token LessOperator
%token GreaterOrEqualOperator
%token GreaterOperator
%token SubstractOperator
%token MultiplicationOperator
%token AssignmentOperator
%token DivisionOperator
%token AdditionOperator
%token BooleanAndOperator
%token BooleanOrOperator
%token PackageKeyword
%token Comma
%token Point
%token SemiColon
%token IntegerLiteral
%token BooleanLiteral
%token ClassKeyword
%token PublicKeyword
%token StaticKeyword
%token ExtendsKeyword
%token PrivateKeyword
%token MainKeyword
%token StringKeyword
%token IfStatement
%token ElseStatement
%token WhileStatement
%token PrintLnKeyword
%token LengthGetter
%token ThisKeyword
%token NewKeyword
%token ReturnKeyword
%token LineComment
%token BlockComment
%token VoidKeyword
%token NegationOperator




%start program

%%
program             : mainClass classDeclarationsList
                    | mainClass
                    | mainClass classDeclarationsList error {yyerror ("EOF expected"); }	
                    ;

mainClass           : ClassKeyword Identifier OpenCurlyBracket PublicKeyword StaticKeyword VoidKeyword MainKeyword OpenParentheses StringKeyword OpenBracket ClosedBracket Identifier ClosedParentheses OpenCurlyBracket statement ClosedCurlyBracket ClosedCurlyBracket
                    | ClassKeyword OpenCurlyBracket PublicKeyword StaticKeyword VoidKeyword MainKeyword OpenParentheses StringKeyword OpenBracket ClosedBracket Identifier ClosedParentheses OpenCurlyBracket statement ClosedCurlyBracket ClosedCurlyBracket {yyerror (" error: class name missing"); }
                    | ClassKeyword Identifier PublicKeyword StaticKeyword VoidKeyword MainKeyword OpenParentheses StringKeyword OpenBracket ClosedBracket Identifier ClosedParentheses OpenCurlyBracket statement ClosedCurlyBracket ClosedCurlyBracket {yyerror (" error: { expected"); }
                    | ClassKeyword Identifier OpenCurlyBracket StaticKeyword VoidKeyword MainKeyword OpenParentheses StringKeyword OpenBracket ClosedBracket Identifier ClosedParentheses OpenCurlyBracket statement ClosedCurlyBracket ClosedCurlyBracket {yyerror (" error: access modifier expected"); }
                    | ClassKeyword Identifier OpenCurlyBracket PublicKeyword VoidKeyword MainKeyword OpenParentheses StringKeyword OpenBracket ClosedBracket Identifier ClosedParentheses OpenCurlyBracket statement ClosedCurlyBracket ClosedCurlyBracket {yyerror (" error: main class function needs to be static"); }
                    | ClassKeyword Identifier OpenCurlyBracket PublicKeyword StaticKeyword MainKeyword OpenParentheses StringKeyword OpenBracket ClosedBracket Identifier ClosedParentheses OpenCurlyBracket statement ClosedCurlyBracket ClosedCurlyBracket {yyerror (" error: main function needs a void return type"); }
                    | ClassKeyword Identifier OpenCurlyBracket PublicKeyword StaticKeyword VoidKeyword OpenParentheses StringKeyword OpenBracket ClosedBracket Identifier ClosedParentheses OpenCurlyBracket statement ClosedCurlyBracket ClosedCurlyBracket {yyerror (" error: main function expected"); }
                    ;

classDeclarationsList : classDeclaration classDeclarationsList | ;
classDeclaration    : ClassKeyword Identifier optionalExtendsStatement OpenCurlyBracket optionalVarDeclarations optionalMethodDeclarations ClosedCurlyBracket | ;

optionalExtendsStatement    : ExtendsKeyword Identifier | ;
optionalVarDeclarations     : varDeclaration optionalVarDeclarations | ;
optionalMethodDeclarations  : methodDeclaration optionalMethodDeclarations | ;

varDeclaration      : Type Identifier SemiColon
                    ;

methodDeclaration   : PublicKeyword Type Identifier OpenParentheses optionalParameterList ClosedParentheses OpenCurlyBracket optionalVarDeclarations optionalStatements ReturnKeyword expression SemiColon ClosedCurlyBracket;

statement           : OpenCurlyBracket optionalStatements ClosedCurlyBracket
                    | IfStatement OpenParentheses expression ClosedParentheses statement ElseStatement statement
                    | WhileStatement OpenParentheses expression ClosedParentheses statement
                    | PrintLnKeyword OpenParentheses expression ClosedParentheses SemiColon
                    | Identifier AssignmentOperator expression SemiColon
                    | Identifier OpenBracket expression ClosedBracket AssignmentOperator expression SemiColon
                    ;

optionalParameterList : Type Identifier secondaryParameter | ;
secondaryParameter  : Comma Type Identifier secondaryParameter | ;
optionalStatements  : statement optionalStatements | ;

expression          : expression operator expression
                    | expression OpenBracket expression ClosedBracket
                    | expression Point LengthGetter
                    | expression Point Identifier OpenParentheses optionalExpressions ClosedParentheses
                    | IntegerLiteral
                    | BooleanLiteral
                    | StringLiteral
                    | Identifier
                    | ThisKeyword
                    | NewKeyword Type OpenBracket expression ClosedBracket
                    | NewKeyword Identifier OpenParentheses ClosedParentheses
                    | NegationOperator expression
                    | OpenParentheses expression ClosedParentheses
                    ;

secondaryExpressions    : Comma expression secondaryExpressions | ;
optionalExpressions     : expression secondaryExpressions | ;

operator            : BooleanAndOperator | BooleanOrOperator | LessOperator | LessOrEqualOperator | EqualityOperator | InequalityOperator | GreaterOperator | GreaterOrEqualOperator | AdditionOperator | SubstractOperator | MultiplicationOperator | DivisionOperator;
%% 



int yyerror(char const *msg) {
       
	
	fprintf(stderr, "%s %d\n", msg,yylineno);
	return 0;
	
	
}

extern FILE *yyin;

int main()
{
 yyparse();
 
 
}