#ifndef PRETRANS_H_AOB8PHQT
#define PRETRANS_H_AOB8PHQT
#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <list>
#include <regex>
#include <sstream>
#include "symbol_store.h"
using namespace std;
class pretrans
{
public:
	pretrans (symbol_store* symbol);
	virtual ~pretrans ();
	void pretranslate(fstream& PlainSrc, fstream& ObjFile);

private:
	symbol_store* _symbol_store;
	typedef string (pretrans::*PreTrans)(stringstream&, const smatch&);
	list<regex> codeblock_pattern;
	list<pair<regex, PreTrans>> pretranslate_pattern;
	stringstream get_codeblock(istream& src);
	void break_codeblock(stringstream& codeblock, stack<list<string> >&result);
	string pretranslate_codeblock(stringstream codeblock);
	string pretrans_assign(stringstream& codeblock, const smatch& sm);
	string pretrans_compare(stringstream& codeblock, const smatch& sm);
	string pretrans_if(stringstream& codeblock, const smatch& sm);
	string pretrans_while(stringstream& codeblock, const smatch& sm);

};
#endif /* end of include guard: PRETRANS_H_AOB8PHQT */
