//============================================================================
// Name        : calc.cpp
// Author      : bkumpar
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Evaluator.h"

int main() {
	std::wcout << L"command line calculator" << std::endl;
	Evaluator e;
	std::wstring expression = L"sin(max(2,3 )/3*PI())";
	e.parse(expression);
	double res = 0;
	std::wcout << res << std::endl;
	return 0;
}
