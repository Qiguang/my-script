#include <string>
#include <map>
#include <stdint.h>

using namespace std;
class symbol_store
{
public:
	symbol_store ();
	virtual ~symbol_store ();
	enum result	{
		SUCCESS,
		ERR_NAME_CONFLICT,
		ERR_NO_THAT,
	};
	uint8_t get_var_loc(string name);
	uint8_t get_const_loc(string name);
	bool get_func_token(string name, uint8_t& token);
private:
#define MY_INSTRUCTION(name, token) token,
	enum func_token {
		#include "api_defs.h"  
	};
#undef MY_INSTRUCTION
	map<string/* const value */, uint8_t/* location */> const_store;
	uint8_t const_num;
	map<string/* variable name */, uint8_t/* location*/> var_store;
	uint8_t var_num;
	map<string/* function name*/, uint8_t/* token */> func_store;
	#if 0
	result store_a_var(string name);
	result store_a_const(uint32_t value);
	#endif
};
