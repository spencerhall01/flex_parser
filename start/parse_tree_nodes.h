//*****************************************************************************
// (part 4)
// purpose: node classes used while building a parse tree for TIPS language
// version: Fall 2023
//  author: John Spencer Hall (jsh278)
//*****************************************************************************

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <map>
#include <vector>
#include "lexer.h"

using namespace std;

// Holds the symbols in the interpreted program
typedef map<string, float> symbolTableT;
extern symbolTableT symbolTable;

extern bool printDelete; // shall we print deleting the tree?

// ---------------------------------------------------------------------
// Forward declaration of node types
class ProgramNode;
class StmtNode;
class BlockNode;
class ExprNode;
class SimpExprNode;
class TermNode;
class FactorNode;

// ---------------------------------------------------------------------
class ProgramNode {
public:
	int _level = 0;
	BlockNode* _block;
	
	ProgramNode(int level);
	~ProgramNode();
	void interpret();
};
ostream& operator<<(ostream&, ProgramNode&);
// ---------------------------------------------------------------------
// Abstract class.
// Base class for AssignNode, CompoundNode, IfNode, WhileNode, ReadNode, and WriteNode.
class StmtNode {
public:
    int _level = 0;
	
    virtual ~StmtNode();
	virtual void interpret();
	virtual void printTo(ostream &os) = 0;
};
ostream& operator<<(ostream&, StmtNode&);
// ---------------------------------------------------------------------
class AssignNode : public StmtNode {
public:
	string* _id = nullptr;
    ExprNode* _expr = nullptr;
	
    AssignNode(int level);
    ~AssignNode();
	void interpret();
	void printTo(ostream &os);
};
// ---------------------------------------------------------------------
class CompoundNode : public StmtNode {
public:
    vector<StmtNode*> _stmts;
	
    CompoundNode(int level);
    ~CompoundNode();
	void interpret();
	void printTo(ostream &os);
};
// ---------------------------------------------------------------------
class BlockNode {
public:
    int _level = 0;
    CompoundNode* _compoundStmt = nullptr;
	
    BlockNode(int level);
    ~BlockNode();
	void interpret();
};
ostream& operator<<(ostream&, BlockNode&);
// ---------------------------------------------------------------------
class IfNode : public StmtNode {
public:
    ExprNode* _expr = nullptr;
	StmtNode* _thenStmt = nullptr;
	StmtNode* _elseStmt = nullptr;
	
    IfNode(int level);
    ~IfNode();
	void interpret();
	void printTo(ostream &os);
};
// ---------------------------------------------------------------------
class WhileNode : public StmtNode {
public:
    ExprNode* _expr = nullptr;
	StmtNode* _stmt = nullptr;
	
    WhileNode(int level);
    ~WhileNode();
	void interpret();
	void printTo(ostream &os);
};
// ---------------------------------------------------------------------
class ReadNode : public StmtNode {
public:
    string* _id = nullptr;
	
    ReadNode(int level);
    ~ReadNode();
	void interpret();
	void printTo(ostream &os);
};
// ---------------------------------------------------------------------
class WriteNode : public StmtNode {
public:
	string* _id = nullptr;
    string* _string = nullptr;
	
    WriteNode(int level);
    ~WriteNode();
	void interpret();
	void printTo(ostream &os);
};
// ---------------------------------------------------------------------
class ExprNode {
public:
  int _level = 0;
  SimpExprNode* _firstSimpExpr = nullptr;
  int _compareOp = 0; // equality, less than, greater than, not equal
  SimpExprNode* _secondSimpExpr = nullptr;

  ExprNode(int level);
  ~ExprNode();
  float interpret();
};
ostream& operator<<(ostream&, ExprNode&);
// ---------------------------------------------------------------------
class SimpExprNode {
public:
  int _level = 0;
  TermNode* _firstTerm = nullptr;
  vector<int> _termOps; // add, subtract, OR
  vector<TermNode*> _restTerms;

  SimpExprNode(int level);
  ~SimpExprNode();
  float interpret();
};
ostream& operator<<(ostream&, SimpExprNode&);
// ---------------------------------------------------------------------
class TermNode {
public:
	int _level = 0;
	FactorNode* _firstFactor = nullptr;
	vector<int> _factorOps; // multiply, divide, AND
	vector<FactorNode*> _restFactors;
	
	TermNode(int level);
	~TermNode();
	float interpret();
};
ostream& operator<<(ostream&, TermNode&);
// ---------------------------------------------------------------------
// Abstract class.
// Base class for IntLitNode, FloatLitNode, IdNode, NestedExprNode, NotNode, and MinusNode.
class FactorNode {
public:
	int _level = 0;
	
	virtual ~FactorNode();
	virtual float interpret() = 0;
	virtual void printTo(ostream &os) = 0;
};
ostream& operator<<(ostream&, FactorNode&);
// ---------------------------------------------------------------------
class IntLitNode : public FactorNode {
public:
    int _int_lit = 0;
	
    IntLitNode(int level, int int_lit);
    ~IntLitNode();
	float interpret();
	void printTo(ostream & os);
};
// ---------------------------------------------------------------------
class FloatLitNode : public FactorNode {
public:
    float _float_lit = 0;
	
    FloatLitNode(int level, float float_lit);
    ~FloatLitNode();
	float interpret();
	void printTo(ostream & os);
};
// ---------------------------------------------------------------------
class IdNode : public FactorNode {
public:
    string* _id = nullptr;
	
    IdNode(int level, string id);
    ~IdNode();
	float interpret();
	void printTo(ostream & os);
};
// ---------------------------------------------------------------------
class NestedExprNode : public FactorNode {
public:
    ExprNode* _expr = nullptr;
	
    NestedExprNode(int level, ExprNode* expr);
    ~NestedExprNode();
	float interpret();
	void printTo(ostream & os);
};
// ---------------------------------------------------------------------
class NotNode : public FactorNode {
public:
    FactorNode* _factor = nullptr;
	
    NotNode(int level, FactorNode* factor);
    ~NotNode();
	float interpret();
	void printTo(ostream & os);
};
// ---------------------------------------------------------------------
class MinusNode : public FactorNode {
public:
    FactorNode* _factor = nullptr;
	
    MinusNode(int level, FactorNode* factor);
    ~MinusNode();
	float interpret();
	void printTo(ostream & os);
};

#endif /* PARSE_TREE_NODES_H */
