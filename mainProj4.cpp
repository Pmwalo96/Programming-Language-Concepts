#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include "parP4.h"
int lineNumber;
string fileMessage;
bool aner = false;
int errorcount = 0;
void messageStart(int linenum) {
	cout << fileMessage << linenum + 1 << ":";
}

void j(ParseTree *m) {
	if (m == 0)
		return;
	if (m->getLeft() != 0)
	{
		j(m->getLeft());
	}
	if (m->getRight() != 0)
	{
		j(m->getRight());
	}
	m->eval();
}

void error(int linenum, const string& message) {
	++errorcount;
	messageStart(linenum);
	cout << "Syntax error " << message << endl;
}
void k(ParseTree *n) {
	if (n == 0)
		return;
	if (n->getLeft() != 0)
	{
		k(n->getLeft());
	}
	if (n->getRight() != 0)
	{
		k(n->getRight());
	}
	n->findSemanticErrors();
}

void semanticError(int linenum, const string& message) {
	messageStart(linenum);
	cout << message << endl;
}

map<string, Value> SymbolTable;

int main(int argc, char *argv[])
{
	ifstream file;
	istream *in = &cin;


	for (int i = 1; i < argc; i++) {
		string arg(argv[i]);
		fileMessage = arg + ":";
		file.open(arg);
		if (file.is_open() == false) {
			cout << arg << " FILE NOT FOUND" << endl;
			return 0;
		}

		in = &file;
	}

	ParseTree *tree = Prog(in);
	k(tree);
	if (aner != true){j(tree);}
	extern map<string, Value>	SymbolTable;
	return 0;
}
