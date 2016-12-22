#include "symbol_store.h"

symbol_store::symbol_store () : const_num(0), var_num(0){
#define MY_INSTRUCTION(name, token) ins_token_store.insert(make_pair(name, static_cast<uint8_t>(token)));
#include "api_defs.h"
#undef MY_INSTRUCTION
#define MY_INSTRUCTION(name, token) ins_str_store.insert(make_pair(static_cast<uint8_t>(token), name));
#include "api_defs.h"
#undef MY_INSTRUCTION
}
symbol_store::~symbol_store () {

}

uint8_t symbol_store::get_var_loc(string name) {
	uint8_t location;
	auto iter = var_store.find(name);
	if (iter != var_store.end()) {
		location = iter->second;
	} else {
		var_store[name] = location = var_num++;
	}

	return location;
}
uint8_t symbol_store::get_const_loc(string name) {
	uint8_t location;
	auto iter = const_store.find(name);
	if (iter != const_store.end()) {
		location = iter->second;
	} else {
		const_store[name] = location = const_num++;
	}
	return location;
}
bool symbol_store::get_ins_token(string name, uint8_t& token) {
	auto iter = ins_token_store.find(name);
	if (iter != ins_token_store.end()) {
		token = iter->second;
		return true;
	}
	return false;
}
string symbol_store::get_ins_str(INS_TOKEN token) {
	auto iter = ins_str_store.find(static_cast<uint8_t>(token));
	if (iter != ins_str_store.end()) {
		return iter->second;
	}
	return "";
}
