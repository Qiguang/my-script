#ifndef SYMBOL_STORE_H_WHUF6DVA
#define SYMBOL_STORE_H_WHUF6DVA

#include <string>
#include <map>
#include <stdint.h>

using namespace std;
class symbol_store
{
public:
#define MY_INSTRUCTION(name, token) token,
	enum INS_TOKEN {
		#include "api_defs.h"  
	};
#undef MY_INSTRUCTION
#define MY_API(name, token, ptr, num) token,
	enum API_TOKEN { 
		#include "api_defs.h"   
	};
#undef MY_API
	symbol_store ();
	virtual ~symbol_store ();
	enum result	{
		SUCCESS,
		ERR_NAME_CONFLICT,
		ERR_NO_THAT,
	};
	uint8_t get_var_offs(string name);
	uint8_t get_const_offs(string name);
	bool get_ins_token(string name, uint8_t& token);
	string get_ins_str(INS_TOKEN token);
	bool get_api_token(const string name, uint8_t& token);
private:
	map<string/* const value */, uint8_t/* location */> const_store;
	uint8_t const_num;
	map<string/* variable name */, uint8_t/* location*/> var_store;
	uint8_t var_num;
	map<string/* instruction name*/, uint8_t/* token */> ins_token_store;
	map<uint8_t/* enum of instructions */, string/* instruction name */>ins_str_store;
	map<string/* api name */, uint8_t/* api token */> api_token_store;
};


#endif /* end of include guard: SYMBOL_STORE_H_WHUF6DVA */
