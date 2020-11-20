//============================================================================
// Name        : calc.cpp
// Author      : bkumpar
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include "Evaluator.h"

int main() {
	std::wcout << L"command line calculator" << std::endl;
	Evaluator e;
	std::vector<std::wstring> expressions { L"sin(max(2,3)/3*pi())",
			L"10+12+33",
			L"12/3/2",
			L"2^3*4^5+6=",
			L"sqrt(9)" };
	for ( size_t i = 0; i < expressions.size(); i++) {
		std::wstring expression = expressions[i] ;
		e.parse(expression);
		double res = e.result();
		std::wstring rpn = e.rpn();
		std::wcout << expression << std::endl;
		std::wcout << rpn << std::endl;
		std::wcout << res << std::endl;
	}
	return 0;
}
