#ifndef PRETRANS_H_AOB8PHQT
#define PRETRANS_H_AOB8PHQT
#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <list>
#include <regex>
#include <sstream>
#include <stdint.h>
#include "symbol_store.h"
using namespace std;
class pretrans
{
public:
	pretrans (symbol_store* symbol);
	virtual ~pretrans ();
	void pretranslate(istream& PlainSrc, ostream& ObjFile);

private:
	symbol_store* _symbol_store;
	typedef string (pretrans::*PreTrans)(const stringstream&, const smatch&);
	typedef stringstream (pretrans::*FlowCtrlPreTrans)(const stringstream& boundary, const smatch sm, const stringstream& translated_content);
	typedef void (pretrans::*separate)(smatch sm, stringstream& flow_ctrl, stringstream& codeblock_content);
	uint32_t label_counter;
	list<regex> codeblock_pattern;
	list<pair<regex, PreTrans>> statement_pattern;
	list<pair<regex, FlowCtrlPreTrans>>flow_ctrl_pattern;
	list<pair<regex, separate>>codeblock_break;
	bool get_codeblock(istream& src, stringstream& codeblock);
	void break_codeblock(stringstream& codeblock, stack<list<string> >&result);
	stringstream separate_content_boundary(const stringstream& codeblock, stringstream& codeblock_content);
	stringstream trans_boundary(const stringstream& boundary, const stringstream& translated_content);
	string pretranslate_codeblock(stringstream codeblock);
	stringstream trans_statement(const stringstream& statement);
	string pretrans_assign(const stringstream& codeblock, const smatch& sm);
	string pretrans_compare(const stringstream& codeblock, const smatch& sm);
	stringstream pretrans_if(const stringstream& boundary, const smatch sm, const stringstream& translated_content);
	stringstream pretrans_while(const stringstream& boundary, const smatch sm, const stringstream& translated_content);
	string get_new_label();
	void separate_if(smatch sm, stringstream& flow_ctrl, stringstream& codeblock_content);
	void separate_while(smatch sm, stringstream& flow_ctrl, stringstream& codeblock_content);
};
#endif /* end of include guard: PRETRANS_H_AOB8PHQT */
