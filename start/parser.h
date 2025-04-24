/*********************************************************************
  Name: Spencer Hall             NetID: jsh278
  Course: CSE 4714              
  Project: 02 - Parser 
  Purpose of File: Contains the function declartions for the parser.
**********************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include "parse_tree_nodes.h"
#include <string>
#include <iostream>

using namespace std;

extern int nextToken;        // next token returned by lexer
extern bool printParse;      // shall tree be printed while parsing?

extern "C" {
	// Instantiate global variables used by flex
	extern int   yylex();      // the generated lexical analyzer
	extern char* yytext;       // text of current lexeme
}

// Function declarations
int lex();
ProgramNode* program();
BlockNode* block();
StmtNode* statement();
AssignNode* assignment();
CompoundNode* compound_stmt();
IfNode* if_stmt();
WhileNode* while_stmt();
ReadNode* read_stmt();
WriteNode* write_stmt();
ExprNode* expression();
SimpExprNode* simple_expr();
TermNode* term();
FactorNode* factor();

#endif /* PARSER_H */
