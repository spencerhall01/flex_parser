//*****************************************************************************
// (part 4)
// purpose: driver file for TIPS interpreter
// version: Fall 2023
//  author: John Spencer Hall (jsh278)
//*****************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <cstring>
#include "lexer.h"
#include "parser.h"
#include "parse_tree_nodes.h"

using namespace std;

extern "C"
{
  // Instantiate global variables
  extern FILE *yyin;        // input stream
  extern FILE *yyout;       // output stream
  extern int   yyleng;      // length of current lexeme
  extern char *yytext;      // text of current lexeme
  extern int   yylineno;    // line number for current lexeme
  extern int   yylex();     // the generated lexical analyzer
  extern int   yylex_destroy(); // deletes memory allocated by yylex
}

//*****************************************************************************
// The main processing loop
//
int main(int argc, char* argv[])
{
  // Whether to print these items
  bool printTree = false; 
  bool printSymbolTable = false;
  
  // Process any command-line switches
  for(int i = 1; i < argc; i++) {
    // -p flag: if requested, print while parsing
    if(strcmp(argv[i], "-p") == 0) {
      printParse = !printParse;
    }
    // -t flag: if requested, print parse tree
    if(strcmp(argv[i], "-t") == 0) {
      printTree = !printTree;
    }
	// -s flag: if requested, print symbol table
    if(strcmp(argv[i], "-s") == 0) {
      printSymbolTable = true;
    }
    // -d flag: if requested, print while deleting parse tree
    if(strcmp(argv[i], "-d") == 0) {
      printDelete = !printDelete;
    }
  }
  // Set the input stream
  if (argc > 1) {
    if(printParse)
		printf("INFO: Using the %s file for input\n", argv[argc-1]);
    yyin = fopen(argv[argc-1], "r"); 
  }
  // Otherwise, flex sets yyin to be stdin, the standard input
  
  // There are three ways to execute the program.  All of these can make use of
  // the command-line switches shown above.

  // (1) The file name can be given on the command line:
  //     $ parse.exe front.in
  //     $ parse.exe -d -p front.in
  
  // (2) After starting the program, an expression can be typed interactively.
  //     If so, to end must type "Ctrl-D" (generates EOF character). (Note, as of
  //     Fall 2023, in the terminal integrated with Visual Studio Code, sending 
  //     an EOF character in this way no longer works. But it still works when 
  //     using the regular terminal.app.)
  //     $ parse.exe
  //     2 + sum
  //     ^D

  // (3) Files or strings can be sent into the parse with the pipe operator, like
  //     this:
  //     echo "5 + 3" | parse.exe
  //     echo "5 + 3" | parse.exe -p -d
  //     cat front.in | parse.exe
  //     cat front.in | parse.exe -p -d

  // Create the root of the parse tree
  ProgramNode* root = nullptr;

  // Fire up the parser!
  lex();
  try {
    root = program();
	if (nextToken != TOK_EOF)
      throw "end of file expected, but there is more here!";
	root->interpret();
  } catch (char const *errmsg) {
      cout << endl << "***ERROR:" << endl;
      cout << "On line number " << yylineno << ", near |" << yytext << "|, error type ";
      cout << errmsg << endl;
      return EXIT_FAILURE;
  }

  if (yyin)
    fclose(yyin);

  yylex_destroy();

  // Printing and Deleting the tree all result in 
  // the same in-order traversal of the tree as parsing.  All
  // use the call stack.
  
  // Tell the world about our success!!
  if(printParse) {
	cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;
  }
  
  if(printTree) {
    cout << endl << "*** Print the Tree ***" << endl;
    cout << *root << endl << endl;
  }
  
  // Print out the symbol table
  if(printSymbolTable) {
	cout << endl << "*** User Defined Symbols ***" << endl;
	symbolTableT::iterator it;
	for (it = symbolTable.begin(); it != symbolTable.end(); ++it) {
		cout << it->first << "\t" << it->second << endl;
	}
	cout << endl;
  }
  
  // Print deletion of tree
  if(printDelete) {
	cout << "*** Delete the Tree ***" << endl;
	delete root;
	root = nullptr;
  }
  
  return(EXIT_SUCCESS);
}
