#include "script_translator.h"
#include <regex>
script_translator::script_translator(string src_name) : m_SrcName(src_name), m_initalized(false) {
	if (m_SrcName.empty()) return;
	size_t pos = m_SrcName.find(".mys");
	if (string::npos == pos) {
		std::cout << "not a .mys file" << std::endl;
		return ;
	}
	string NameWithoutSuffix = m_SrcName.substr(0, m_SrcName.length()-pos);
	m_PlainSrcName = NameWithoutSuffix + ".psrc";
	m_ObjName = NameWithoutSuffix + ".myobj";
	m_BinName = NameWithoutSuffix + ".mybin";
	initInternalData();
	m_initalized = true;
}
script_translator::~script_translator() {
// TODO: close all files	
}

void script_translator::translate() {
	if (!m_initalized) return;
	preprocess();
	pretranslate();
	trans2bin();
}

void script_translator::initInternalData() {
	InsTranlator.insert(make_pair("ADD", &script_translator::ADD_2Bin));
	InsTranlator.insert(make_pair("ASSIGN", &script_translator::ASSIGN_2Bin));
}

/**************************************************************************************************
*  remove comments
*  remove white spaces
*  store const value defined by #define and remove them
*  generate plain src file  *
**************************************************************************************************/

void script_translator::preprocess() {
	std::cout << __FUNCTION__ << std::endl;
	fstream SrcFile;
	fstream PlainSrc;
	if (!OpenFile(SrcFile, m_SrcName) || !CreateFile(PlainSrc, m_PlainSrcName)) {
		CloseFile(SrcFile);
		CloseFile(PlainSrc);
		return;
	}
	string line;

	do {
		getline(SrcFile, line); 
		std::cout << line << std::endl;
		// TODO remove white spaces
		regex e("\\s+");
		line = regex_replace(line, e, string(""), regex_constants::match_any);
		PlainSrc << line << std::endl;
	} while (SrcFile.good());
	
	CloseFile(SrcFile);
	CloseFile(PlainSrc);
}

/*****************************************************************
*  open plain src file
*  check syntax
*  convert script syntax to function call
*  generate obj file  *
*****************************************************************/

void script_translator::pretranslate() {
	//TODO check syntax
	std::cout << __FUNCTION__ << std::endl;
	fstream PlainSrc;
	fstream ObjFile;
	if (!OpenFile(PlainSrc, m_PlainSrcName) || !CreateFile(ObjFile, m_ObjName)) {
		CloseFile(PlainSrc);
		CloseFile(ObjFile);
		return;
	}

	string line;
	do {
		getline(PlainSrc, line); 
		std::cout << line << std::endl;
		regex e("^([_a-zA-Z0-9])+=(\\d+)$");
		smatch sm;
		if (!regex_match(line, sm, e)) {
			continue;
		}
		std::cout << sm.size() << "matches in this line" << std::endl;
		std::cout << "they are: " << std::endl;
		for (size_t i = 0; i < sm.size(); ++i) {
			std::cout << sm[i] << std::endl;
		}
		ObjFile << "ASSIGN";
		for (size_t i = 1; i < sm.size(); ++i) {
			ObjFile << " " << sm[i];
		}
		ObjFile << std::endl;
	} while (PlainSrc.good());
	
	CloseFile(ObjFile);
	CloseFile(PlainSrc);
}

/****************************************************************************
*  open obj file
*  translate the function calls to bin
*  generate myexe  *
****************************************************************************/

void script_translator::trans2bin() {
	std::cout << __FUNCTION__ << std::endl;
	fstream ObjFile;
	fstream BinFile;
	if (!OpenFile(ObjFile, m_ObjName) || !CreateFile(BinFile, m_BinName)) {
		CloseFile(ObjFile);
		CloseFile(BinFile);
		return;
	}
	string line;
	do {
		getline(ObjFile, line);
		regex e("^(\\w+)\\s");
		smatch sm;
		if (!regex_search(line, sm, e)) continue;
		auto iter = InsTranlator.find(sm[1]);
		vector<uint8_t> bin_trans_result;
		if (iter == InsTranlator.end()) {
			std::cout << "unknown instruction at TODO line" << std::endl;
			continue;
		}
		
		bool success = (this->*(iter->second))(line, bin_trans_result);
		if (!success) {
			std::cout << "cannot translate line TODO" << std::endl;
			continue;
		}
			
		BinFile.write((const char*)bin_trans_result.data(), bin_trans_result.size());
	} while (ObjFile.good());
	CloseFile(ObjFile);
	CloseFile(BinFile);
}

bool script_translator::OpenFile(fstream& stream, string& Name) {
	stream.open(Name);
	if (!stream.is_open()) {
		cout << "cannot open file: "	<< Name << endl;
		return false;
	}
	return true;
}

bool script_translator::CloseFile(fstream& stream) {
	if (stream.is_open()) {
		stream.close();
	}
	return true;
}


bool script_translator::CreateFile(fstream& stream, string& Name) {
	ofstream newfile(Name);
	if (!newfile.is_open()) {
		std::cout << "create file: [" << Name << "] failed" << std::endl;
		return false;
	}
	newfile.close();
	OpenFile(stream, Name);	
	return true;
}
bool script_translator::ADD_2Bin(const string& instruction, vector<uint8_t>& bin) {
	return true;
}
bool script_translator::ASSIGN_2Bin(const string& instruction, vector<uint8_t>& bin) {
	regex e("^(.+)\\s+(.+)\\s(.+)$");
	smatch sm;
	regex_match(instruction, sm, e);
	uint8_t func_token;
	_symbol_store.get_func_token(sm[1], func_token);
	bin.push_back(func_token);
	uint8_t var_loc = _symbol_store.get_var_loc(sm[2]);
	bin.push_back(var_loc);
	uint8_t const_loc = _symbol_store.get_const_loc(sm[3]);
	bin.push_back(const_loc);
	return true;
}
