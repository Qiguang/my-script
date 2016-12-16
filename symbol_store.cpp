#include "symbol_store.h"

symbol_store::symbol_store () : var_num(0), const_num(0){
#define MY_INSTRUCTION(name, token) func_store.insert(make_pair(name, static_cast<uint8_t>(token)));
#include "api_defs.h"
#undef MY_INSTRUCTION
}
symbol_store::~symbol_store () {

}

#if 0


symbol_store::result symbol_store::store_a_var(string name) {
	auto iter = var_store.find(name);
	if (iter == var_store.end()) {
		var_store[name] = var_num++;
		return SUCCESS;
	}
	return ERR_;
}
symbol_store::result symbol_store::store_a_const(uint32_t value) {

	return SUCCESS;
}
#endif

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
bool symbol_store::get_func_token(string name, uint8_t& token) {
	auto iter = func_store.find(name);
	if (iter != func_store.end()) {
		token = iter->second;
		return true;
	}
	return false;

}
