#ifndef SCRIPT_TRANSLATOR_H_PE19QAZ4
#define SCRIPT_TRANSLATOR_H_PE19QAZ4

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <stdint.h>
#include "symbol_store.h"
#include <list>
#include <regex>
#include <stack>
#include <sstream>
using namespace std;

class script_translator
{
public:
	script_translator (string src_name);
	virtual ~script_translator ();
	void translate();

private:
	string m_SrcName;
	string m_PlainSrcName;
	string m_ObjName;
	string m_BinName;
	void preprocess();
	void pretranslate();
	void trans2bin();
	bool m_initalized;
	bool OpenFile(fstream& stream, string& Name);
	bool CreateFile(fstream& stream, string& Name);
	bool CloseFile(fstream& stream);
	typedef bool (script_translator::*Ins2Bin)(const string&, vector<uint8_t>&);
	map<string/* instructions */, Ins2Bin /* parameter handler*/> InsTranlator;
	bool ADD_2Bin(const string& instruction, vector<uint8_t>& bin);
	bool MOV_2Bin(const string& instruction, vector<uint8_t>& bin);
	void replace_defination(string& line);
	void remove_whitespace(string& line);
	void initInternalData();
	void syntax_check();
	bool isString(string& str);
	bool isConst(string& str);
	bool isLabelRef(string& str);
	bool Obj2Bin(const string& instruction, vector<uint8_t>& bin);
	stringstream get_codeblock(istream& src);
	void break_codeblock(stringstream& codeblock, stack<list<string> >&result);
	string pretranslate_codeblock(stringstream codeblock);
	symbol_store _symbol_store;
	string pretrans_assign(stringstream& codeblock, const smatch& sm);
	string pretrans_compare(stringstream& codeblock, const smatch& sm);
	string pretrans_if(stringstream& codeblock, const smatch& sm);
	string pretrans_while(stringstream& codeblock, const smatch& sm);
};


#endif /* end of include guard: SCRIPT_TRANSLATOR_H_PE19QAZ4 */
