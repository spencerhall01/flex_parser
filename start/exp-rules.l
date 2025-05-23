/*********************************************************************
 Name: John Spencer Hall
 NetID: jsh278
 Course: CSE 4714
 Assignment: Part 1 - Lexical Analyzer for TIPS Language
 Programming Environment: Flex (Lexical Analyzer Generator)
 Purpose of File: Lexical analysis rules for TIPS language.
*********************************************************************/

%option yylineno
%option noyywrap

%{
#include "lexer.h"
%}

%%
BEGIN				{ return TOK_BEGIN; }
BREAK				{ return TOK_BREAK; }
CONTINUE			{ return TOK_CONTINUE; }
DOWNTO				{ return TOK_DOWNTO; }
ELSE				{ return TOK_ELSE; }
END					{ return TOK_END; }
FOR					{ return TOK_FOR; }
IF					{ return TOK_IF; }
LET					{ return TOK_LET; }
PROGRAM				{ return TOK_PROGRAM; }
READ				{ return TOK_READ; }
THEN				{ return TOK_THEN; }
TO					{ return TOK_TO; }
VAR					{ return TOK_VAR; }
WHILE				{ return TOK_WHILE; }
WRITE				{ return TOK_WRITE; }

INTEGER				{ return TOK_INTEGER; }
REAL				{ return TOK_REAL; }

;					{ return TOK_SEMICOLON; }
:					{ return TOK_COLON; }
\(					{ return TOK_OPENPAREN; }
\)					{ return TOK_CLOSEPAREN; }

\+					{ return TOK_PLUS; }
-					{ return TOK_MINUS; }
\*					{ return TOK_MULTIPLY; }
\/					{ return TOK_DIVIDE; }
:=					{ return TOK_ASSIGN; }
=					{ return TOK_EQUALTO; }
\<					{ return TOK_LESSTHAN; }
>					{ return TOK_GREATERTHAN; }
\<>					{ return TOK_NOTEQUALTO; }
MOD					{ return TOK_MOD; }
NOT					{ return TOK_NOT; }
OR					{ return TOK_OR; }
AND					{ return TOK_AND; }

[A-Z][A-Z0-9]*		{ if (strlen(yytext) <= 8) { return TOK_IDENT; } return TOK_UNKNOWN; }
[0-9]+				{ return TOK_INTLIT; }
[0-9]+\.[0-9]+		{ return TOK_FLOATLIT; }
'[^'\n]*'			{ if (strlen(yytext) <= 82) { return TOK_STRINGLIT; } return TOK_UNKNOWN; }

[ \n\r\t]+			{ /* Ignore whitespace */ }
"{"[^\n]*"}"		{ /* Ignore comments */ }
"(*".*"*)"			{ /* Ignore comments */ }

.					{ return TOK_UNKNOWN; }

<<EOF>>				{ return TOK_EOF; }
