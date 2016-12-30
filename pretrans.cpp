#include "pretrans.h"

pretrans::pretrans (symbol_store* symbol): _symbol_store(symbol),  label_counter(0){
	codeblock_pattern.push_back(regex("^if\\(.+\\)"));
	codeblock_pattern.push_back(regex("^while\\(.+\\)"));

	// a = b
	statement_pattern.push_back(make_pair(regex("^([_A-Za-z][_A-Za-z0-9]*)\\s*=\\s*(.+)$"), &pretrans::pretrans_assign));
	// < <= > >=
	statement_pattern.push_back(make_pair(regex("^(.+)\\s*([<>]=?)\\s*(.+)$"), &pretrans::pretrans_compare));
	// if
	flow_ctrl_pattern.push_back(make_pair(regex("^if\\s*\\(\\s*(.+)\\s*\\).*$"), &pretrans::pretrans_if));
	// while
	flow_ctrl_pattern.push_back(make_pair(regex("^while\\s*\\(\\s*(.+)\\s*\\).*$"), &pretrans::pretrans_while));

	codeblock_break.push_back(make_pair(regex("^\\s*(if\\s*\\(.+\\))\\s*\\{\\s*((?:.+\\s*)+)\\}"), &pretrans::separate_if));

	codeblock_break.push_back(make_pair(regex("^\\s*(while\\s*\\(.+\\))\\s*\\{\\s*((?:.+\\s*)+)\\}"), &pretrans::separate_while));
}
pretrans::~pretrans () {

}
void pretrans::pretranslate(istream& PlainSrc, ostream& ObjFile) {
	string line;
	stringstream translated_codeblock;
	smatch sm;
	stringstream translated;
	while (PlainSrc.good()) {
			stringstream codeblock;
			if (get_codeblock(PlainSrc, codeblock)) {
				stringstream codeblock_content;
				stringstream codeblock_boundary;
				stringstream translated_content;
				codeblock_boundary = separate_content_boundary(codeblock, codeblock_content);
				pretranslate(codeblock_content, translated_content);
				translated = trans_boundary(codeblock_boundary, translated_content);
			} else {
				translated = trans_statement(codeblock);
			}
			if (translated.good())
				translated_codeblock << translated.str();
	}
	ObjFile << translated_codeblock.str();
}
stringstream pretrans::trans_statement(const stringstream& statement) {
	smatch sm;
	stringstream ret;
	string str;
	str = statement.str();
	if (str.empty()) {
		return ret;
	}
	for (auto i = statement_pattern.begin(); i != statement_pattern.end(); ++i) {
		if (regex_match(str, sm, i->first) && i->second) {
			ret << (this->*(i->second))(statement, sm) << endl;
			return ret;
		}
	}
	// it is not a internal statement, test if it is a function call
	regex e("\\s*(.+)\\((.*)\\)");  // get func name
	if (!regex_match(str, sm, e)) {
		printf("this is a unrecognizable statement [%s]\n", __FUNCTION__ );
		return ret;
	}
	uint8_t dummy;
	if (_symbol_store->get_api_token(sm[1], dummy)) {
		ret << "CALL " << sm[1];
		if (sm[2].length()) {
			string arguments = sm[2].str();
			while (regex_search(arguments, sm, regex("\\s*([^,]+),*\\s*"))) {
				ret << " " << sm[1];
				arguments = sm.suffix().str();
			}
		}
		ret << endl;
	}
	return ret;
}
stringstream pretrans::separate_content_boundary(const stringstream& codeblock, stringstream& codeblock_content) {
	string block = codeblock.str();
	smatch sm;
	
	stringstream ret;

	for (auto i = codeblock_break.begin(); i != codeblock_break.end(); ++i) {
		if (regex_match(block, sm, i->first) && i->second) {
			(this->*(i->second))(sm, ret, codeblock_content);
			break;
		}
	}
	return ret;
}
void pretrans::separate_if(smatch sm, stringstream& flow_ctrl, stringstream& codeblock_content) {
	flow_ctrl << sm[1];
	codeblock_content << sm[2];
}
void pretrans::separate_while(smatch sm, stringstream& flow_ctrl, stringstream& codeblock_content) {
	flow_ctrl << sm[1];
	codeblock_content << sm[2];
}
stringstream pretrans::trans_boundary(const stringstream& boundary, const stringstream& translated_content) {
	stringstream ret;
	string str = boundary.str();
	smatch sm;
	for (auto i = flow_ctrl_pattern.begin(); i != flow_ctrl_pattern.end(); ++i) {
		if (regex_match(str, sm, i->first) && i->second) {
			ret = (this->*(i->second))(boundary, sm, translated_content);
			break;
		}
	}
	return ret;
}
bool pretrans::get_codeblock(istream& src, stringstream& codeblock) {
	printf("[%s][%s]\n", __FUNCTION__,  __FILE__);
	string line;
	getline(src, line);
	smatch sm;
	bool has_codeblock = false;
	for (auto i = codeblock_pattern.cbegin(); i != codeblock_pattern.cend(); ++i) {
		if (regex_search(line, sm, *i)) {
			has_codeblock = true;
			break;
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
				codeblock << line << endl;
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
			codeblock << line;
			if (braces_pair) {
				codeblock << endl;
#if _MY_DEBUG
				if (!src.good()) {
					printf("shouldn't be here, there are programming errors\n[%s][%s][%s]\n",__FILE__, __LINE__, __FUNCTION__);
				}
#endif
				getline(src, line);
			}	
		}
	} else {
		codeblock << line;
	}
	std::cout << codeblock.str() << std::endl;
	return has_codeblock;
}
void pretrans::break_codeblock(stringstream& codeblock, stack<list<string> >&result) {
	
}
string pretrans::pretranslate_codeblock(stringstream codeblock) {
	printf("[%s][%s]\n", __FUNCTION__,  __FILE__);
	smatch sm;
	const string& block = codeblock.str();
	for (auto i = statement_pattern.begin(); i != statement_pattern.end(); ++i) {
		if (regex_match(block, sm, i->first) && i->second) {
			return (this->*(i->second))(codeblock, sm);
		}
	}
	printf("programming error[%s][%s]\n", __FUNCTION__, __FILE__);
	return string("");
}

// a = b
// statement_pattern.push_back(regex("^([_A-Za-z][_A-Za-z0-9]+)\\s*=\\s*(.+)$"));
string pretrans::pretrans_assign(const stringstream& codeblock, const smatch& sm) {
	string mov(_symbol_store->get_ins_str(symbol_store::INS_MOV));
	if (sm.size() < 3) {
		printf("programming error[%s][%s]\n", __FUNCTION__, __FILE__);
		return "";
	}
	return mov + ' ' + sm.str(1) + ' ' + sm.str(2);
}
// < <= > >=
// statement_pattern.push_back(regex("^(.+)\\s*([<>]=?)\\s*(.+)$"));
string pretrans::pretrans_compare(const stringstream& codeblock, const smatch& sm) {
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
// statement_pattern.push_back(regex("^if\\s*\\(\\s*(.+)\\s*\\).*$"));
stringstream pretrans::pretrans_if(const stringstream& boundary, const smatch sm, const stringstream& translated_content) {
	stringstream condition;
	condition << sm[1];
	stringstream ret;
	string label = get_new_label();
	string label_end = get_new_label();
	ret = trans_statement(condition);
	ret << "CGOTO " << label << endl;
	ret << "GOTO " << label_end << endl;
	ret << label << ":" << endl;
	ret << translated_content.str();
	ret << label_end << ":" << endl;
	return ret;
}

// while
// statement_pattern.push_back(regex("^while\\s*\\(\\s*(.+)\\s*\\).*$"));
stringstream pretrans::pretrans_while(const stringstream& boundary, const smatch sm, const stringstream& translated_content) {
	
	stringstream condition;
	condition << sm[1];
	stringstream ret;
	stringstream cond;
	string condition_label = get_new_label();
	string block_label = get_new_label();
	string block_end_label = get_new_label();
	cond = trans_statement(condition);
	ret << condition_label << ":" <<endl;
	ret << cond.str();
	ret << "CGOTO " << block_label << endl;
	ret << "GOTO " << block_end_label << endl;
	ret << block_label << ":" << endl;
	ret << translated_content.str();
	ret << "GOTO " << condition_label << endl;
	ret << block_end_label << ":" << endl;
	return ret;
}
string pretrans::get_new_label() {
	stringstream label;
	label << "#Label" << label_counter;
	++label_counter;
	return label.str();
}
