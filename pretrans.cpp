#include "pretrans.h"

pretrans::pretrans (symbol_store* symbol): _symbol_store(symbol) {
	codeblock_pattern.push_back(regex("^if\\(.+\\)"));
	codeblock_pattern.push_back(regex("^while\\(.+\\)"));

	// a = b
	pretranslate_pattern.push_back(make_pair(regex("^([_A-Za-z][_A-Za-z0-9]*)\\s*=\\s*(.+)$"), &pretrans::pretrans_assign));
	// < <= > >=
	pretranslate_pattern.push_back(make_pair(regex("^(.+)\\s*([<>]=?)\\s*(.+)$"), &pretrans::pretrans_compare));
	// if
	pretranslate_pattern.push_back(make_pair(regex("^if\\s*\\(\\s*(.+)\\s*\\).*$"), nullptr));
	// while
	pretranslate_pattern.push_back(make_pair(regex("^while\\s*\\(\\s*(.+)\\s*\\).*$"), nullptr));
}
pretrans::~pretrans () {

}
void pretrans::pretranslate(fstream& PlainSrc, fstream& ObjFile) {
	pretranslate_codeblock(get_codeblock(PlainSrc));
	
}
stringstream pretrans::get_codeblock(istream& src) {
	printf("[%s][%s]\n", __FUNCTION__,  __FILE__);
	string line;
	getline(src, line);
	smatch sm;
	bool has_codeblock = false;
	stringstream rv;
	for (auto i = codeblock_pattern.cbegin(); i != codeblock_pattern.cend(); ++i) {
		if (regex_search(line, sm, *i)) {
			has_codeblock = true;
		}
	}
	if (has_codeblock) {
		size_t braces_pair = 0;
		size_t pos = 0;
		while (!braces_pair) {
			pos = line.find('{');
			if (pos != string::npos) {
				braces_pair++;
			} else {
				rv << line << endl;
#if _MY_DEBUG
				if (!src.good()) {
					printf("shouldn't be here, there are programming errors\n[%s][%s][%s]\n",__FILE__, __LINE__, __FUNCTION__);
				}
#endif
				getline(src, line);
			}
		}
		while (braces_pair) {
			pos = line.find_first_of("{}", (pos==string::npos ? 0 : pos+1));
			while (pos != string::npos) {
				if (line[pos] == '{') braces_pair++;
				else braces_pair--;
				pos = line.find_first_of("{}", pos+1);
			}
			rv << line;
			if (braces_pair) {
				rv << endl;
#if _MY_DEBUG
				if (!src.good()) {
					printf("shouldn't be here, there are programming errors\n[%s][%s][%s]\n",__FILE__, __LINE__, __FUNCTION__);
				}
#endif
				getline(src, line);
			}	
		}
	} else {
		rv << line;
	}
	std::cout << rv.str() << std::endl;
	return rv;
}
void pretrans::break_codeblock(stringstream& codeblock, stack<list<string> >&result) {
	
}
string pretrans::pretranslate_codeblock(stringstream codeblock) {
	printf("[%s][%s]\n", __FUNCTION__,  __FILE__);
	smatch sm;
	const string& block = codeblock.str();
	for (auto i = pretranslate_pattern.begin(); i != pretranslate_pattern.end(); ++i) {
		if (regex_match(block, sm, i->first) && i->second) {
			return (this->*(i->second))(codeblock, sm);
		}
	}
	printf("programming error[%s][%s]\n", __FUNCTION__, __FILE__);
	return string("");
}

// a = b
// pretranslate_pattern.push_back(regex("^([_A-Za-z][_A-Za-z0-9]+)\\s*=\\s*(.+)$"));
string pretrans::pretrans_assign(stringstream& codeblock, const smatch& sm) {
	string mov(_symbol_store->get_ins_str(symbol_store::INS_MOV));
	if (sm.size() < 3) {
		printf("programming error[%s][%s]\n", __FUNCTION__, __FILE__);
		return "";
	}
	return mov + ' ' + sm.str(1) + ' ' + sm.str(2);
}
// < <= > >=
// pretranslate_pattern.push_back(regex("^(.+)\\s*([<>]=?)\\s*(.+)$"));
string pretrans::pretrans_compare(stringstream& codeblock, const smatch& sm) {
	if (sm.size() < 4) {
		printf("programming error[%s][%s]\n", __FUNCTION__, __FILE__);
		return "";
	}
	string instruction;
	string& sign = sm.str(2);
	if (sign == ">") {
		instruction = _symbol_store->get_ins_str(symbol_store::INS_GREATER_THAN);
	} else if (sign == ">=") {
		instruction = _symbol_store->get_ins_str(symbol_store::INS_GREATER_EQUAL);
	} else if (sign == "<") {
		instruction = _symbol_store->get_ins_str(symbol_store::INS_LESS_THAN);
	} else if (sign == "<=") {
		instruction = _symbol_store->get_ins_str(symbol_store::INS_LESS_EQUAL);
	} else {
		printf("programming error1[%s][%s]\n", __FUNCTION__, __FILE__);
	}
	return instruction + ' ' + sm.str(1) + ' ' + sm.str(3);
}
// if
// pretranslate_pattern.push_back(regex("^if\\s*\\(\\s*(.+)\\s*\\).*$"));
string pretrans::pretrans_if(stringstream& codeblock, const smatch& sm) {
	
	return "";
}

// while
// pretranslate_pattern.push_back(regex("^while\\s*\\(\\s*(.+)\\s*\\).*$"));
string pretrans::pretrans_while(stringstream& codeblock, const smatch& sm) {
	
	return "";
}
