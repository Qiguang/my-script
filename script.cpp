// script.cpp : Defines the entry point for the console application.
//

#include <regex>
#include "script_defs.h"
#include "script_translator.h"

int main(int argc, char* argv[])
{
	if (argc <= 1) {
		std::cout << "need commandline options" << std::endl;
		std::cout << "r for run, t for translate" << std::endl;
		return 0;
	}

	if (!strcmp(argv[1], "r")) { // run the script
		std::cout << "run" << std::endl;

	} else if (!strcmp(argv[1], "t")) { // translate the script
		std::cout << "translate"  << std::endl;
		script_translator translator("main.mys");
		translator.translate();
	} else {
		std::cout << "undefined options" << std::endl;
	}
	return 0;
}

