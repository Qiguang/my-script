#include "script_translator.h"
#include "pretrans.h"

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
	InsTranlator.insert(make_pair("ADD",        &script_translator::ADD_2Bin));
	InsTranlator.insert(make_pair("MOV",        &script_translator::MOV_2Bin));
#if 0
	InsTranlator.insert(make_pair("LT",         &script_translator::LT_2Bin));
	InsTranlator.insert(make_pair("LE",         &script_translator::LE_2Bin));
	InsTranlator.insert(make_pair("GT",         &script_translator::GT_2Bin));
	InsTranlator.insert(make_pair("GE",         &script_translator::GE_2Bin));
	InsTranlator.insert(make_pair("CALL",       &script_translator::CALL_2Bin));
	InsTranlator.insert(make_pair("CGOTO",      &script_translator::CGOTO_2Bin));
	InsTranlator.insert(make_pair("GOTO",       &script_translator::GOTO_2Bin));
#endif
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
		replace_defination(line);
		remove_whitespace(line);
		if (line.empty()) continue;
		PlainSrc << line << std::endl;
	} while (SrcFile.good());

	CloseFile(SrcFile);
	CloseFile(PlainSrc);
}

void script_translator::replace_defination(string& line) {
	// TODO: 
}

void script_translator::remove_whitespace(string& line) {
	regex e("\\s+");
	line =  regex_replace(line, e, string(""), regex_constants::match_any);
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
#if 0  // syntax check has not implemented currently
	do {
		syntax_check();
	} while (PlainSrc.good());
#endif

	pretrans _pretrans(&_symbol_store);
	_pretrans.pretranslate(PlainSrc, ObjFile);
	
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
		regex e("#?\\b\\w+\\b|\"[^\"]*\"");	
		smatch sm;
		uint8_t binary = 0;
		// is it a label definition?
		string label_name;
		if (isLabelDef(line, label_name)) _symbol_store.store_label(label_name);
		else
		while (regex_search(line, sm, e)) {
			// if it is a string
			if (isString(sm[0].str())) {
				// add to string store
			} else if (isConst(sm[0].str())) { // if it is a const
				binary = _symbol_store.get_const_offs(sm[0]);
			} else if (isLabelRef(sm[0].str(), label_name)) { // if it is a label reference
				if (!_symbol_store.get_label_offs(sm[0], binary)) {
					unsolved_label.push_back(make_pair(label_name, BinFile.tellp));
					binary = 0;
				}
			// else it should be a instruction or a api call
			} else if (_symbol_store.get_ins_token(sm[0], binary)) {
				
			} else if (_symbol_store.get_api_token(sm[0], binary)) {
				
			} else {
				// error: unknown symbol
				printf("unknown symbol [%s]\n", __FUNCTION__);
			}
			BinFile.write(&binary, sizeof(binary));
			line = sm.suffix();
		}
			
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
bool script_translator::Obj2Bin(const string& instruction, vector<uint8_t>& bin) {
	regex e("\\b\\w+\\b:?|\"[^\"]*\"");	
	smatch sm;
	uint8_t binary = 0;
	string str = instruction;
	while (regex_search(str, sm, e)) {
		// if it is a string
		if (isString(sm[0].str())) {
			// add to string store
		} else if (isConst(sm[0].str())) { // if it is a const
			binary = _symbol_store.get_const_offs(sm[0]);
		} else if (isLabelRef(sm[0].str())) { // if it is a label
			binary = _symbol_store.get_label_offs(sm[0]);
		// else it should be a instruction or a api call
		} else if (_symbol_store.get_ins_token(sm[0], binary)) {
			
		} else if (_symbol_store.get_api_token(sm[0], binary)) {
			
		} else {
			// error: unknown symbol
			printf("unknown symbol [%s]\n", __FUNCTION__);
		}
		bin.push_back(binary);
		str = sm.suffix();
	}

}
bool script_translator::isString(string& str) {
	if (!str.empty() && str.front()=='"' && str.end()=='"') return true;
	return false;
}
bool script_translator::isConst(string& str) {
	if (!str.empty() && regex_match(str, regex("^\\d+$"))) return true;
	return false;
}
bool script_translator::isLabelRef(string& str, string& label_name) {
	if (!str.empty() && str.front()=='#') {
		label_name=str.substr(sizeof('#'));
		return true;
	}
	return false;
}
bool script_translator::isLabelDef(string& str, string& label_name) {
	if (!str.empty() && str.front()==':') {
		label_name=str.substr(0, str.length()-sizeof(':'));
		return true;
	}
	return false;
}
bool script_translator::ADD_2Bin(const string& instruction, vector<uint8_t>& bin) {
	return true;
}
bool script_translator::MOV_2Bin(const string& instruction, vector<uint8_t>& bin) {
	regex e("^(.+)\\s+(.+)\\s(.+)$");
	smatch sm;
	regex_match(instruction, sm, e);
	uint8_t func_token;
	_symbol_store.get_ins_token(sm[1], func_token);
	bin.push_back(func_token);
	uint8_t var_offs = _symbol_store.get_var_offs(sm[2]);
	bin.push_back(var_offs);
	uint8_t const_offs = _symbol_store.get_const_offs(sm[3]);
	bin.push_back(const_offs);
	return true;
}
#if 0

bool script_translator::LT_2Bin(const string& instruction, vector<uint8_t>& bin) {
	return true;
}
bool script_translator::LE_2Bin(const string& instruction, vector<uint8_t>& bin) {
	return true;
}
bool script_translator::GT_2Bin(const string& instruction, vector<uint8_t>& bin) {
	return true;
}
bool script_translator::GE_2Bin(const string& instruction, vector<uint8_t>& bin) {
	return true;
}
bool script_translator::CALL_2Bin(const string& instruction, vector<uint8_t>& bin) {
	return true;
}
bool script_translator::CGOTO_2Bin(const string& instruction, vector<uint8_t>& bin) {
	return true;
}
bool script_translator::GOTO_2Bin(const string& instruction, vector<uint8_t>& bin) {
	return true;
}

#endif
void script_translator::syntax_check() {
	printf("[%s][%s]\n", __FUNCTION__,  __FILE__);

}
