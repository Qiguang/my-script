#ifdef MY_INSTRUCTION
	//             ins name   ins token     argument num
	MY_INSTRUCTION("ADD",     INS_ADD      )
	MY_INSTRUCTION("MOV",     INS_MOV)
	MY_INSTRUCTION("LT",      INS_LESS_THAN)
	MY_INSTRUCTION("LE",      INS_LESS_EQUAL)
	MY_INSTRUCTION("GT",      INS_GREATER_THAN)
	MY_INSTRUCTION("GE",      INS_GREATER_EQUAL)
	MY_INSTRUCTION("CALL",    INS_CALL)
	MY_INSTRUCTION("CGOTO",   INS_CGOTO)
	MY_INSTRUCTION("GOTO",    INS_GOTO)
#endif

#ifdef MY_API
	//     name      index       func ptr  parameter num
	MY_API("printf", API_PRINTF, printf,       -1)
	MY_API("func_a", API_FUNC_A, func_a,        0)
	MY_API("func_b", API_FUNC_B, func_b,        1)
#endif

