#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
using std::istream;

#include <string>
#include <map>
using std::string;
using std::stoi;
using std::map;

#include "lexer.h"
extern bool aner;
extern void error(int linenum, const string& message);
extern void semanticError(int linenum, const string& message);
enum TypeForNode { INT_TYPE, STRING_TYPE, ERROR_TYPE, PRINT_TYPE };
class Value {
	TypeForNode	type;
	int			ival;
	string		sval;

public:
    Value(string r) : sval(r) { type = STRING_TYPE; }
    Value(int m) : ival(m) { type = INT_TYPE; }
	Value(TypeForNode t = ERROR_TYPE) : type(t), ival(0) {}
	TypeForNode GetType() const { return type; }

	int GetIntValue() const {
		if (type == INT_TYPE) return ival;
		throw "This variable is not an int";
	}
	string GetStringValue() const {
		if (type == STRING_TYPE) return sval;
		throw "This variable is not a string";
	}
};

extern map<string, Value> SymbolTable;


class ParseTree {
	int			linenumber;
	ParseTree	*left;
	ParseTree	*right;

public:
	ParseTree(int n, ParseTree *l = 0, ParseTree *r = 0) : linenumber(n), left(l), right(r) {}
	virtual ~ParseTree() {}

	ParseTree* getLeft() const { return left; }
	ParseTree* getRight() const { return right; }
	int getLineNumber() const { return linenumber; }

	virtual TypeForNode GetType() const { return ERROR_TYPE; }
    virtual int findSemanticErrors() const { return 0; }
	virtual Value eval() { return Value(); }
	virtual int GetIntValue() const { throw "no integer value"; }
	virtual string GetStringValue() const { throw "no string value"; }

};

class StatementList : public ParseTree {
public:
	StatementList(ParseTree *first, ParseTree *rest) : ParseTree(0, first, rest) {}

};

class DeclStatement : public ParseTree {
	TypeForNode	type;
	string 		id;

public:
	DeclStatement(int line, TypeForNode ty, string id) : ParseTree(line), type(ty), id(id) {}

	int findSemanticErrors() const {
		if (SymbolTable.find(id) != SymbolTable.end()) {
			semanticError(this->getLineNumber(), "variable " + id + " was already declared");
			return 1;
		}
		SymbolTable[id] = Value(type);
		return 1;
	}

};

class SetStatement : public ParseTree {
	string id;

public:
	SetStatement(int line, string id, ParseTree *ex) : ParseTree(line, ex), id(id) {}
	int countSet() const { return 1; }

	Value eval()
	{
      char a =  getLeft()->eval().GetType(); char b =  SymbolTable[this->id].GetType();
	   if (b == a)
		{
			SymbolTable[id]=Value(getLeft()->eval());
		}
		return Value();
	}

	int findSemanticErrors() const {
     
		if (SymbolTable.find(id) == SymbolTable.end()) {
			semanticError(this->getLineNumber(), "variable " + id + " is used before being declared");
			return 1;
		}
        char e = SymbolTable[this->id].GetType();
        char f = getLeft()->eval().GetType();
        int g = this->getLineNumber();
		if (f != e)
			semanticError(g, "type error");
		return 0;
	}
};

class PrintStatement : public ParseTree {
	bool addNL;

public:
	PrintStatement(int line, bool isPrintln, ParseTree *ex) : ParseTree(line, ex), addNL(isPrintln) {}
	TypeForNode GetType() const { return PRINT_TYPE; }

	void doprinting() const {
		if (addNL) std::cout << std::endl;
	}

	Value eval()
	{
        char a = getLeft()->eval().GetType();
		if (a == STRING_TYPE) {std::cout << getLeft()->eval().GetStringValue();}
		else if (a == INT_TYPE) {std::cout << getLeft()->eval().GetIntValue();}
		doprinting();
		return Value();
	}
};

class Addition : public ParseTree {
public:
	Addition(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

	int findSemanticErrors() const {
        char r = getRight()->eval().GetType();
        int q = this->getLineNumber();
        char l = getLeft()->eval().GetType();
		if ((r != l))
		{
            aner = true;
			semanticError(q, "type error");
		}
		return 0;
	}
	Value eval() {
        char r = getRight()->eval().GetType();
        char l = getLeft()->eval().GetType();
		if (r == INT_TYPE && l == INT_TYPE){return Value(getRight()->eval().GetIntValue() + getLeft()->eval().GetIntValue());}
		else if (l == STRING_TYPE && r == STRING_TYPE)
           return Value(getRight()->eval().GetStringValue() + getLeft()->eval().GetStringValue());
		return Value();
	}
};

class Subtraction : public ParseTree {
public:
	Subtraction(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

	int findSemanticErrors() const {
        char l = getLeft()->eval().GetType();
        char r = getRight()->eval().GetType();
        int nl = this->getLineNumber();
		if (l == STRING_TYPE && r == INT_TYPE)
		{
			semanticError(nl, "type error");
            aner = true;
			return 0;
		}
		return 0;
	}

	Value eval() {
        char l = getLeft()->eval().GetType();
        char r = getRight()->eval().GetType();
		if (l == INT_TYPE && r == INT_TYPE)
		{
			return Value(getLeft()->eval().GetIntValue() - getRight()->eval().GetIntValue());
		}

		return Value();
	}
};
class Multiplication : public ParseTree {
public:
	Multiplication(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

	int findSemanticErrors() const {
        char l = getLeft()->eval().GetType();
        char r = getRight()->eval().GetType();
        int y = this->getLineNumber();
		if ((l == STRING_TYPE && r == STRING_TYPE))
		{
			aner = true;
            semanticError(y, "type error");
		}
		return 0;
	}
	Value eval() {
		string w;
        char l = getLeft()->eval().GetType();
        char r = getRight()->eval().GetType();
		if (l == INT_TYPE && r == INT_TYPE)
		{
			return Value(getRight()->eval().GetIntValue() * getLeft()->eval().GetIntValue());
		}

		else if (l == INT_TYPE && r == STRING_TYPE)
		{
            for(int c=0; c<getLeft()->eval().GetIntValue();c++)
            {
               w += getRight()->eval().GetStringValue();
            }
			return Value(w);
		}
		else if ((l == STRING_TYPE && r == INT_TYPE))
		{
            for(int c=0; c<getRight()->eval().GetIntValue();c++)
            {
               w += getLeft()->eval().GetStringValue();
            }
			return Value(w);
		}
		return Value();
	}
};

class Division : public ParseTree {
public:
	Division(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

	int findSemanticErrors() const {
        char l = getLeft()->eval().GetType();
        char r = getRight()->eval().GetType();
        int y = this->getLineNumber();
		if ((l == INT_TYPE && r == STRING_TYPE))
		{
            aner = true;
			semanticError(y, "type error");
		}
		else if ((l == STRING_TYPE && r == INT_TYPE))
		{
			aner = true;
			semanticError(y, "type error");
		}
		return 0;
	}

	Value eval()
	{
        char l = getLeft()->eval().GetType();
        char r = getRight()->eval().GetType();
		string a;
        string b;
		if (l == INT_TYPE && r == INT_TYPE){return Value(getLeft()->eval().GetIntValue() / getRight()->eval().GetIntValue());}
		else if (l == STRING_TYPE && r == STRING_TYPE)
		{
			a = getLeft()->eval().GetStringValue(); b = getRight()->eval().GetStringValue(); std::size_t lenny=a.find(b);
			if (std::string::npos != lenny){a.replace(a.find(b), b.length(), "");}
			return Value(a);
		}
		return Value();
	}

};

class IntegerConstant : public ParseTree {
	int	value;

public:
	IntegerConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
		value = stoi(tok.GetLexeme());
	}
	TypeForNode GetType() const { return INT_TYPE; }
	int GetIntValue() const { return value; }
    Value eval() {
		return Value(value);
	}
};

class StringConstant : public ParseTree {
	string	value;

public:
	StringConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
		value = tok.GetLexeme();
		value = value.substr(1, value.length() - 2);
	}

	Value eval() {
		return Value(value);
	}
};

class Identifier : public ParseTree {
	string	value;

public:
	Identifier(const Token& tok) : ParseTree(tok.GetLinenum()) {
		value = tok.GetLexeme();
	}
	int findSemanticErrors() const {
		if (SymbolTable.find(value) == SymbolTable.end()) {
			semanticError(this->getLineNumber(), "variable " + value + " is used before being declared");
			aner = true;
			return 1;
		}
		return 0;
	}
	Value eval() {
		return Value(SymbolTable[value]);
	}
};

extern ParseTree *	Prog(istream* in);
extern ParseTree *	StmtList(istream* in);
extern ParseTree *	Stmt(istream* in);
extern ParseTree *	Decl(istream* in);
extern ParseTree *	Set(istream* in);
extern ParseTree *	Print(istream* in);
extern ParseTree *	Expr(istream* in);
extern ParseTree *	Term(istream* in);
extern ParseTree *	Primary(istream* in);


#endif /* PARSER_H_ */
