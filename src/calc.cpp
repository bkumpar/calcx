//============================================================================
// Name        : calc.cpp
// Author      : bkumpar
// Version     :
// Copyright   :
// Description : Command line program which evaluate mathematical expressions
//============================================================================

#include <iostream>
#include <vector>
#include "Evaluator.h"

void printHelp() {
	std::wcout << L"command line calculator\n"
			<< L"calc.exe [options] expression" << std::endl;
}

void runTests() {
	Evaluator e;
	std::vector<std::wstring> expressions { L"((2+3)*5)+(3*2)",  L"((2+3)*5)+5", L"(2+3+4)*5",  L"sqrt(9)", L"sin(max(2,3)/3*pi())",
			L"10+12+33", L"12/3/2", L"2^3*4^5+6="};
	for (size_t i = 0; i < expressions.size(); i++) {
		std::wstring expression = expressions[i];
		e.parse(expression);
		double res = e.result();
		std::wstring rpn = e.rpn();
		std::wcout << expression << L"\t" << rpn << L"\t" << res << std::endl;
	}
}

int main( int argc, char* argv[]) {
	if (argc == 1) {
		printHelp();
		return 0;
	}

	std::string str(argv[1]);
	std::wstring arg(str.begin(), str.end());

	if (arg == L"-t")
	{
		runTests();
		return 0;
	} else {
		Evaluator e;
		std::wstring expression(arg);
		e.parse(expression);
		double res = e.result();
		std::wstring rpn = e.rpn();
		std::wcout << res << std::endl;

		return 0;
	}
}
