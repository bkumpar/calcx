//============================================================================
// Name        : calc.cpp
// Author      : bkumpar
// Version     :
// Copyright   :
// Description : Command line program which evaluate mathematical expressions
//============================================================================

#include <iostream>
#include <vector>
#include <getopt.h>
#include "Evaluator.h"

void printHelp() {
	std::wcout << L"command line calculator\n"
			<< L"Usage: calc [options] expression\n"
			<< L" -h --help display this information.\n"
			<< L" -t --tests run some examples\n"
			<< L" -r --rpn print expression in RPN notation.\n"
			<< L"note: expression must be \"double quoted\" \n" << std::endl;
}

void runTests() {
	Evaluator e;
	std::vector<std::wstring> expressions { L"sto((5+6),1)", L"sto(13,2)",
			L"rcl(2)+rcl(1)", L"((2+3)*5)+(3*2)", L"((2+3)*5)+5", L"(2+3+4)*5",
			L"sqrt(9)", L"sin(max(2,3)/3*pi())", L"10+12+33", L"12/3/2",
			L"2^3*4^5+6" };
	for (size_t i = 0; i < expressions.size(); i++) {
		std::wstring expression = expressions[i];
		e.parse(expression);
		double res = e.result();
		std::wstring rpn = e.rpn();
		std::wcout << expression << L"\t" << rpn << L"\t" << res << std::endl;
	}
}

int main(int argc, char *argv[]) {

	char nextOption;
	const char *const shortOptions = "htr";
	const struct option longOptions[] = { { "help", 0, NULL, 'h' }, { "test", 0,
			NULL, 't' }, { "rpn", 0, NULL, 'r' }, { NULL, 0, NULL, 0 } };

// show help if no arguments
	if (argc == 1) {
		printHelp();
	}
	bool showRpn = false;
	do {
		nextOption = getopt_long(argc, argv, shortOptions, longOptions, NULL);
		switch (nextOption) {
		case 'h':
			printHelp();
			break;
		case 't':
			runTests();
			break;
		case 'r':
			showRpn = true;
			break;
		}
	} while (nextOption != -1);

	std::wstring expression;
	while (optind < argc) { // optind is GLOBAL (extern)
		std::string str(argv[optind]);
		std::wstring expression(str.begin(), str.end());
		Evaluator e;
		e.parse(expression);
		if (showRpn) {
			std::wcout << e.rpn() << std::endl;
		}
		std::wcout << e.result() << std::endl;
		optind++;
	}

	return 0;
}

