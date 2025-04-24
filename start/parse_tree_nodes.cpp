//*****************************************************************************
// (part 4)
// purpose: node classes used while building a parse tree for TIPS language
// version: Fall 2023
//  author: John Spencer Hall (jsh278)
//*****************************************************************************

#include "parse_tree_nodes.h"

#define EPSILON 0.001

bool printDelete = false; // shall we print deleting the tree?

// ---------------------------------------------------------------------
// Indent according to tree level
static void indent(int level) {
	for (int i = 0; i < level; i++)
		cout << ("|  ");
}
// ---------------------------------------------------------------------
ProgramNode::ProgramNode(int level) {
	_level = level;
};
ProgramNode::~ProgramNode() {
	if(printDelete) 
		cout << "Deleting ProgramNode" << endl;
	delete _block;
	_block = nullptr;
}
void ProgramNode::interpret() {
	_block->interpret();
}
ostream& operator<<(ostream& os, ProgramNode& pn) {
	os << endl; indent(pn._level); os << "(program ";
		os << *(pn._block);
	os << endl; indent(pn._level); os << "program) ";
	return os;
}
// ---------------------------------------------------------------------
BlockNode::BlockNode(int level) {
	_level = level;
};
BlockNode::~BlockNode() {
	if(printDelete) 
		cout << "Deleting BlockNode" << endl;
	delete _compoundStmt;
	_compoundStmt = nullptr;
}
void BlockNode::interpret() {
	_compoundStmt->interpret();
}
ostream& operator<<(ostream& os, BlockNode& bn) {
	os << endl; indent(bn._level); os << "(block ";
		os << *(bn._compoundStmt);
	os << endl; indent(bn._level); os << "block) ";
	return os;
}
// ---------------------------------------------------------------------
StmtNode::~StmtNode() {}

void StmtNode::interpret() {
	this->interpret();
}
// Uses double dispatch to call the overloaded method printTo in the 
// StmtNodes: AssignNode, CompoundNode, IfNode, WhileNode, ReadNode, and WriteNode
ostream& operator<<(ostream& os, StmtNode& sn) {
	sn.printTo(os);
	return os;
}
// ---------------------------------------------------------------------
AssignNode::AssignNode(int level) {
	_level = level;
};
AssignNode::~AssignNode() {
	if(printDelete) 
		cout << "Deleting StatementNode:AssignmentStmtNode" << endl;
	delete _expr;
	_expr = nullptr;
}
void AssignNode::interpret() {
	symbolTableT::iterator it = symbolTable.find(*_id);
	it->second = _expr->interpret();
}
void AssignNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(assignment_stmt ( " << *_id << " := )";
		os << *_expr;
	os << endl; indent(_level); os << "assignment_stmt)";
}
// ---------------------------------------------------------------------
CompoundNode::CompoundNode(int level) {
	_level = level;
};
CompoundNode::~CompoundNode() {
	if(printDelete) 
		cout << "Deleting StatementNode:CompoundStmtNode" << endl;
	for (int i = 0; i < _stmts.size(); ++i) {
		delete _stmts[i];
		_stmts[i] = nullptr;
	}
}
void CompoundNode::interpret() {
	for (int i = 0; i < _stmts.size(); ++i)
		_stmts[i]->interpret();
}
void CompoundNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(compound_stmt";
		for (int i = 0; i < _stmts.size(); ++i)
			os << *(_stmts[i]);
	os << endl; indent(_level); os << "compound_stmt)";
}
// ---------------------------------------------------------------------
IfNode::IfNode(int level) {
	_level = level;
};
IfNode::~IfNode() {
	if(printDelete) 
		cout << "Deleting StatementNode:IfStmtNode" << endl;
	delete _expr;
	_expr = nullptr;
	delete _thenStmt;
	_thenStmt = nullptr;
	delete _elseStmt;
	_elseStmt = nullptr;
}
void IfNode::interpret() {
	if(_expr->interpret() != 0.0)
		_thenStmt->interpret();
	else if(_elseStmt != nullptr)
		_elseStmt->interpret();
}
void IfNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(if_stmt ";
		os << *_expr;
	os << endl; indent(_level); os << "(then ";
		os << *_thenStmt;
	os << endl; indent(_level); os << "then) ";
	
	if(_elseStmt != nullptr) {
		os << endl; indent(_level); os << "(else ";
			os << *_elseStmt;
		os << endl; indent(_level); os << "else) ";
	}
	
	os << endl; indent(_level); os << "if_stmt)";
}
// ---------------------------------------------------------------------
WhileNode::WhileNode(int level) {
	_level = level;
};
WhileNode::~WhileNode() {
	if(printDelete) 
		cout << "Deleting StatementNode:WhileStmtNode" << endl;
	delete _expr;
	_expr = nullptr;
	delete _stmt;
	_stmt = nullptr;
}
void WhileNode::interpret() {
	while(_expr->interpret() != 0.0)
		_stmt->interpret();
}
void WhileNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(while_stmt ";
		os << *_expr;
		os << *_stmt;
	os << endl; indent(_level); os << "while_stmt)";
}
// ---------------------------------------------------------------------
ReadNode::ReadNode(int level) {
	_level = level;
};
ReadNode::~ReadNode() {
	if(printDelete) 
		cout << "Deleting StatementNode:ReadStmtNode" << endl;
}
void ReadNode::interpret() {
	symbolTableT::iterator it = symbolTable.find(*_id);
	cin >> it->second;
}
void ReadNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(read_stmt ( " << *_id << " )";
	os << endl; indent(_level); os << "read_stmt)";
}
// ---------------------------------------------------------------------
WriteNode::WriteNode(int level) {
	_level = level;
};
WriteNode::~WriteNode() {
	if(printDelete) 
		cout << "Deleting StatementNode:WriteStmtNode" << endl;
}
void WriteNode::interpret() {
	if(_id != nullptr) {
		symbolTableT::iterator it = symbolTable.find(*_id);
		cout << it->second << endl;
	} else if (_string != nullptr)
		cout << _string->substr(1, _string->length()-2) << endl;
}
void WriteNode::printTo(ostream& os) {
	if(_id != nullptr) {
		os << endl; indent(_level); os << "(write_stmt ( " << *_id << " )";
		os << endl; indent(_level); os << "write_stmt)";
	} else if(_string != nullptr) {
		os << endl; indent(_level); os << "(write_stmt ( " << *_string << " )";
		os << endl; indent(_level); os << "write_stmt)";
	}
}
// ---------------------------------------------------------------------
ExprNode::ExprNode(int level) {
	_level = level;
}
ExprNode::~ExprNode() {
	if(printDelete)
		cout << "Deleting ExpressionNode" << endl;
	delete _firstSimpExpr;
	_firstSimpExpr = nullptr;
	delete _secondSimpExpr;
	_secondSimpExpr = nullptr;
}
float ExprNode::interpret() {
	switch (_compareOp) {
		case TOK_EQUALTO:
			if((_firstSimpExpr->interpret() - _secondSimpExpr->interpret()) <= EPSILON) return 1.0;
			else return 0.0;
		case TOK_LESSTHAN:
			if(_firstSimpExpr->interpret() < _secondSimpExpr->interpret()) return 1.0;
			else return 0.0;
		case TOK_GREATERTHAN:
			if(_firstSimpExpr->interpret() > _secondSimpExpr->interpret()) return 1.0;
			else return 0.0;
		case TOK_NOTEQUALTO:
			if((_firstSimpExpr->interpret() - _secondSimpExpr->interpret()) > EPSILON) return 1.0;
			else return 0.0;
		default:
			return _firstSimpExpr->interpret();
	}
}
ostream& operator<<(ostream& os, ExprNode& en) {
	os << endl; indent(en._level); os << "(expression ";
		os << *(en._firstSimpExpr);
		
	if (en._compareOp == TOK_EQUALTO) {
		os << endl; indent(en._level); os << "= ";
		os << *(en._secondSimpExpr);
    } else if (en._compareOp == TOK_LESSTHAN) {
		os << endl; indent(en._level); os << "< ";
		os << *(en._secondSimpExpr);
    } else if (en._compareOp == TOK_GREATERTHAN) {
		os << endl; indent(en._level); os << "> ";
		os << *(en._secondSimpExpr);
    } else if (en._compareOp == TOK_NOTEQUALTO){
		os << endl; indent(en._level); os << "<> ";
		os << *(en._secondSimpExpr);
	}
	
	os << endl; indent(en._level); os << "expression) ";
	return os;
}
// ---------------------------------------------------------------------
SimpExprNode::SimpExprNode(int level) {
	_level = level;
}
SimpExprNode::~SimpExprNode() {
	if(printDelete)
		cout << "Deleting SimpleExpNode" << endl;
	delete _firstTerm;
	_firstTerm = nullptr;

	for (int i = 0; i < _termOps.size(); ++i) {
		delete _restTerms[i];
		_restTerms[i] = nullptr;
	}
}
float SimpExprNode::interpret() {
	float result = _firstTerm->interpret();
	for (int i = 0; i < _termOps.size(); ++i) {
		switch (_termOps[i]) {
			case TOK_PLUS:
				result += _restTerms[i]->interpret();
				break;
			case TOK_MINUS:
				result -= _restTerms[i]->interpret();
				break;
			case TOK_OR:
				result = result || _restTerms[i]->interpret();
				break;
		}
	}
	return result;
}
ostream& operator<<(ostream& os, SimpExprNode& sen) {
	os << endl; indent(sen._level); os << "(simple_exp ";
		os << *(sen._firstTerm);
		
	for (int i = 0; i < sen._termOps.size(); ++i) {
		if (sen._termOps[i] == TOK_PLUS) {
			os << endl; indent(sen._level); os << "+ ";
		} else if (sen._termOps[i] == TOK_MINUS) {
			os << endl; indent(sen._level); os << "- ";
		} else if (sen._termOps[i] == TOK_OR) {
			os << endl; indent(sen._level); os << "OR ";
		}
		os << *(sen._restTerms[i]);
	}
	
	os << endl; indent(sen._level); os << "simple_exp) ";
	return os;
}
// ---------------------------------------------------------------------
TermNode::TermNode(int level) {
	_level = level;
}
TermNode::~TermNode() {
	if(printDelete)
		cout << "Deleting TermNode" << endl;
	delete _firstFactor;
	_firstFactor = nullptr;

	for (int i = 0; i < _factorOps.size(); ++i) {
		delete _restFactors[i];
		_restFactors[i] = nullptr;
	}
}
float TermNode::interpret() {
	float result = _firstFactor->interpret();
	for (int i = 0; i < _factorOps.size(); ++i) {
		switch (_factorOps[i]) {
			case TOK_MULTIPLY:
				result *= _restFactors[i]->interpret();
				break;
			case TOK_DIVIDE:
				result /= _restFactors[i]->interpret();
				break;
			case TOK_AND:
				result = result && _restFactors[i]->interpret();
				break;
		}
	}
	return result;
}
ostream& operator<<(ostream& os, TermNode& tn) {
	os << endl; indent(tn._level); os << "(term ";
		os << *(tn._firstFactor);
		
	for (int i = 0; i < tn._factorOps.size(); i++) {
		if (tn._factorOps[i] == TOK_MULTIPLY) {
			os << endl; indent(tn._level); os << "* ";
		} else if (tn._factorOps[i] == TOK_DIVIDE) {
			os << endl; indent(tn._level); os << "/ ";
		} else if (tn._factorOps[i] == TOK_AND) {
			os << endl; indent(tn._level); os << "AND ";
		}
		os << *(tn._restFactors[i]);
	}
	
	os << endl; indent(tn._level); os << "term) ";
	return os;
}
// ---------------------------------------------------------------------
FactorNode::~FactorNode() {
}
float FactorNode::interpret() {
	return this->interpret();
}
ostream& operator<<(ostream& os, FactorNode& fn) {
	os << endl; indent(fn._level); os << "(factor ";
	fn.printTo(os);
	os << endl; indent(fn._level); os << "factor) ";
	return os;
}
// ---------------------------------------------------------------------
IntLitNode::IntLitNode(int level, int int_lit) {
	_level = level;
	_int_lit = int_lit;
}
IntLitNode::~IntLitNode() {
	if(printDelete)
		cout << "Deleting FactorNode:IntLitNode" << endl;
}
float IntLitNode::interpret() {
	return _int_lit; 
}
void IntLitNode::printTo(ostream& os) {
	os << "( INTLIT: " << _int_lit << " ) ";
}
// ---------------------------------------------------------------------
FloatLitNode::FloatLitNode(int level, float float_lit) {
	_level = level;
	_float_lit = float_lit;
}
FloatLitNode::~FloatLitNode() {
	if(printDelete)
		cout << "Deleting FactorNode:FloatLitNode" << endl;
}
float FloatLitNode::interpret() {
	return _float_lit; 
}
void FloatLitNode::printTo(ostream& os) {
	os << "( FLOATLIT: " << _float_lit << " ) ";
}
// ---------------------------------------------------------------------
IdNode::IdNode(int level, string name) {
	_level = level;
	_id = new string(name);
}
IdNode::~IdNode() {
	if(printDelete) 
		cout << "Deleting FactorNode:IdNode" << endl;
	delete _id;
	_id = nullptr;
}
float IdNode::interpret() {
	symbolTableT::iterator it = symbolTable.find(*_id);
	return it->second;
}
void IdNode::printTo(ostream& os) {
	os << "( IDENT: " << *_id << " ) ";
}
// ---------------------------------------------------------------------
NestedExprNode::NestedExprNode(int level, ExprNode* expr) {
	_level = --level;
	_expr = expr;
}
NestedExprNode::~NestedExprNode() {
	if(printDelete)
		cout << "Deleting FactorNode:NestedExprNode" << endl;
	delete _expr;
	_expr = nullptr;
}
float NestedExprNode::interpret() {
	return _expr->interpret();
}
void NestedExprNode::printTo(ostream& os) {
	os << "( ";
		os << *_expr << ")";
}
// ---------------------------------------------------------------------
NotNode::NotNode(int level, FactorNode* factor) {
	_level = --level;
	_factor = factor;
}
NotNode::~NotNode() {
	if(printDelete)
		cout << "Deleting FactorNode:NotNode" << endl;
	delete _factor;
	_factor = nullptr;
}
float NotNode::interpret() {
	return !(_factor->interpret());
}
void NotNode::printTo(ostream& os) {
	os << "(NOT ";
		os << *_factor << ") ";
}
// ---------------------------------------------------------------------
MinusNode::MinusNode(int level, FactorNode* factor) {
	_level = --level;
	_factor = factor;
}
MinusNode::~MinusNode() {
	if(printDelete)
		cout << "Deleting FactorNode:MinusNode" << endl;
	delete _factor;
	_factor = nullptr;
}
float MinusNode::interpret() {
	return -(_factor->interpret());
}
void MinusNode::printTo(ostream& os) {
	os << "(- ";
		os << *_factor << ") ";
}
