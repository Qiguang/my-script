#ifndef SCRIPT_TRANSLATOR_H_PE19QAZ4
#define SCRIPT_TRANSLATOR_H_PE19QAZ4

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <stdint.h>
#include "symbol_store.h"

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
	bool ASSIGN_2Bin(const string& instruction, vector<uint8_t>& bin);
	void initInternalData();
	symbol_store _symbol_store;
};


#endif /* end of include guard: SCRIPT_TRANSLATOR_H_PE19QAZ4 */
