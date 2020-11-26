/*
 * Evaluator.cpp
 *
 *  Created on: 15. stu 2020.
 *      Author: bkumpar
 */

#include <math.h>
#include <exception>
#include <iostream>
#include <fstream>
#include "Evaluator.h"

using std::sqrt;

Evaluator::Evaluator() {
	init();
	_memoryFileName = "./.calc.mem";
	loadMemory();
}

Evaluator::~Evaluator() {
	saveMemory();
}

void Evaluator::saveMemory()
{
	  std::ofstream mem(_memoryFileName.c_str(), std::ios::out | std::ios::binary);
	  mem.write((char*)&memory, sizeof(memory));
	  mem.close();
}

void Evaluator::loadMemory()
{
	  std::ifstream mem(_memoryFileName.c_str(), std::ios::in | std::ios::binary);
	  mem.read((char*)&memory, sizeof(memory));
	  mem.close();
}

double Evaluator::result() {
	return _valueStack.top();
}
std::wstring Evaluator::rpn() {
	return _output;
}

void Evaluator::init() {
	_operators[L"+"] = MathOperator<FunctionPtr>(2, left, &Evaluator::add);
	_operators[L"-"] = MathOperator<FunctionPtr>(2, left, &Evaluator::sub);
	_operators[L"*"] = MathOperator<FunctionPtr>(3, left, &Evaluator::mul);
	_operators[L"/"] = MathOperator<FunctionPtr>(3, left, &Evaluator::div);

	_operators[L"^"] = MathOperator<FunctionPtr>(4, left, &Evaluator::pwr);
	_operators[L"sqrt"] = MathOperator<FunctionPtr>(4, left, &Evaluator::sqrt);
	_operators[L"root"] = MathOperator<FunctionPtr>(4, left, &Evaluator::root);
	_operators[L"sqrt"] = MathOperator<FunctionPtr>(4, left, &Evaluator::sqrt);

	_operators[L"min"] = MathOperator<FunctionPtr>(5, left, &Evaluator::min);
	_operators[L"max"] = MathOperator<FunctionPtr>(5, left, &Evaluator::max);

	_operators[L"sin"] = MathOperator<FunctionPtr>(5, left, &Evaluator::sin);
	_operators[L"cos"] = MathOperator<FunctionPtr>(5, left, &Evaluator::cos);
	_operators[L"tan"] = MathOperator<FunctionPtr>(5, left, &Evaluator::tan);
	_operators[L"asin"] = MathOperator<FunctionPtr>(5, left, &Evaluator::asin);
	_operators[L"acos"] = MathOperator<FunctionPtr>(5, left, &Evaluator::acos);
	_operators[L"atan"] = MathOperator<FunctionPtr>(5, left, &Evaluator::atan);
	_operators[L"pi"] = MathOperator<FunctionPtr>(5, left, &Evaluator::pi);

	_operators[L"exp"] = MathOperator<FunctionPtr>(5, left, &Evaluator::exp);
	_operators[L"ln"] = MathOperator<FunctionPtr>(5, left, &Evaluator::ln);
	_operators[L"log"] = MathOperator<FunctionPtr>(5, left, &Evaluator::log);

	_operators[L"sto"] = MathOperator<FunctionPtr>(5, left, &Evaluator::sto);
	_operators[L"rcl"] = MathOperator<FunctionPtr>(5, left, &Evaluator::rcl);
}
void Evaluator::pushToOutput(std::wstring token) {
	_output += token + L" ";
}

void Evaluator::processFunction(std::wstring token) {
	if (token != L"") {
		_operatorStack.push(token);
	}
}

void Evaluator::procesLeftParenthesis() {
	_operatorStack.push(L"(");
}
void Evaluator::procesRightParenthesis() {
	while (_operatorStack.top() != L"(")
	{
		std::wstring token = _operatorStack.pop();
		pushToOutput(token);
		calc(token);
	}

	if (_operatorStack.top() == L"(") {
		_operatorStack.drop();
	}
}

void Evaluator::processNumber(std::wstring token) {
	if (token != L"") {
		double number = stod(token);
		_valueStack.push(number);
		pushToOutput(token);
	}
}

void Evaluator::processToken(std::wstring token, bool isNumber) {
	if (isNumber) {
		processNumber(token);
	} else {
		processFunction(token);
	}
}

bool Evaluator::isDigit(wchar_t ch) {
	switch (ch) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.':
		return true;
	default:
		return false;
	}
}

bool Evaluator::isOperator(wchar_t ch) {
	switch (ch) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
		return true;
	default:
		return false;
	}
}
bool Evaluator::operatorStackIsNotEmpty() {
	return _operatorStack.isNotEmpty();
}

bool Evaluator::operatorOnStackHasGreaterPrecedence(std::wstring thisOperator) {
	std::wstring opOnStack = _operatorStack.top();
	MathOperatorFn mathOperatorOnStack = _operators[opOnStack];
	MathOperatorFn thisMathOperator = _operators[thisOperator];
	bool result = mathOperatorOnStack > thisMathOperator;
	return result;
}

bool Evaluator::operatorOnStackHasEqualPrecedenceAndLeftAssociativity(
		std::wstring thisOperator) {
	std::wstring opOnStack = _operatorStack.top();
	MathOperatorFn mathOperatorOnStack = _operators[opOnStack];
	MathOperatorFn thisMathOperator = _operators[thisOperator];
	Associativity thisOperatorAssociativity = thisMathOperator.associativity();
	return mathOperatorOnStack == thisMathOperator
			&& (thisOperatorAssociativity == left);
}
bool Evaluator::operatorOnStackIsNotLeftParentheses() {
	std::wstring operatorOnStack = _operatorStack.top();
	return operatorOnStack != L"(";
}

void Evaluator::processOperator(std::wstring op) {
	while ((operatorStackIsNotEmpty())
			&& (operatorOnStackIsNotLeftParentheses())
			&& ((operatorOnStackHasGreaterPrecedence(op))
					|| (operatorOnStackHasEqualPrecedenceAndLeftAssociativity(
							op)))) {
		std::wstring operatorOnStack = _operatorStack.pop();
		pushToOutput(operatorOnStack);
		calc(operatorOnStack);
	}
	_operatorStack.push(op);
}

void Evaluator::finish() {
	while (_operatorStack.isNotEmpty()) {
		std::wstring operatorOnStack = _operatorStack.pop();
		pushToOutput(operatorOnStack);
		calc(operatorOnStack);
	}
}
void Evaluator::calc(std::wstring op) {
	MathOperatorFn mathOperator = _operators[op];
	FunctionPtr function = mathOperator.function();
	(this->*function)();
}

void Evaluator::parse(std::wstring expression) {
	_operatorStack.clear();
	_valueStack.clear();
	_output = L"";
	bool tokenIsNumber = false;
	uint i;
	try {
		std::wstring token = L"";
		for (i = 0; i < expression.length(); i++) {
			wchar_t ch = expression[i];
			if (isDigit(ch)) {
				tokenIsNumber = true;
				token += ch;
			} else if (ch == ',') { // BUG: do not evaluate whole expression before comma!
				if (token != L"") {
					processToken(token, tokenIsNumber);
					tokenIsNumber = false;
					token = L"";
				}
			} else if (ch == '(') {
				if (token != L"") {
					processToken(token, tokenIsNumber);
					tokenIsNumber = false;
					token = L"";
				}
				procesLeftParenthesis();
			} else if (ch == ')') {
				processToken(token, tokenIsNumber);
				tokenIsNumber = false;
				token = L"";
				procesRightParenthesis();
			} else if (isOperator(ch)) {
				processToken(token, tokenIsNumber);
				tokenIsNumber = false;
				token = L"";
				std::wstring op(1, ch);
				processOperator(op);
			} else if (ch == '=') {
				break;
			} else {
				token += ch;
			}
		} // end for

		// process last token
		processToken(token, tokenIsNumber);
		tokenIsNumber = false;
		token = L"";

		finish();
	} catch (...) {
		std::wstring exceptionMessage = L"Error parsing expression "
				+ expression + L" at position " + std::to_wstring(i);
		std::string message(exceptionMessage.begin(), exceptionMessage.end());
		throw std::runtime_error(message);
	} // end try

} // end function

void Evaluator::add() {
	double val1 = _valueStack.pop();
	double val2 = _valueStack.pop();
	double result = val2 + val1;
	_valueStack.push(result);
}

void Evaluator::sub() {
	double val1 = _valueStack.pop();
	double val2 = _valueStack.pop();
	double result = val2 - val1;
	_valueStack.push(result);
}

void Evaluator::mul() {
	double val1 = _valueStack.pop();
	double val2 = _valueStack.pop();
	double result = val2 * val1;
	_valueStack.push(result);
}

void Evaluator::div() {
	double val1 = _valueStack.pop();
	double val2 = _valueStack.pop();
	double result = val2 / val1;
	_valueStack.push(result);
}

void Evaluator::pwr() {
	double val1 = _valueStack.pop();
	double val2 = _valueStack.pop();
	double result = pow(val2, val1);
	_valueStack.push(result);
}

void Evaluator::sqrt() {
	double val1 = _valueStack.pop();
	double result = std::sqrt(val1);
	_valueStack.push(result);
}

void Evaluator::root() {
	double val1 = _valueStack.pop();
	double val2 = _valueStack.pop();
	double result = std:: pow(val1, 1.0/val2);
	_valueStack.push(result);
}

void Evaluator::max() {
	double val1 = _valueStack.pop();
	double val2 = _valueStack.pop();
	double result = val2 > val1 ? val2 : val1;
	_valueStack.push(result);
}

void Evaluator::min() {
	double val1 = _valueStack.pop();
	double val2 = _valueStack.pop();
	double result = val2 < val1 ? val2 : val1;
	_valueStack.push(result);
}

void Evaluator::pi() {
	_valueStack.push(M_PI);
}

void Evaluator::sin() {
	double val1 = _valueStack.pop();
	double result = (double) sinh(val1);
	_valueStack.push(result);
}

void Evaluator::cos() {
	double val1 = _valueStack.pop();
	double result = (double) cosh(val1);
	_valueStack.push(result);
}

void Evaluator::tan() {
	double val1 = _valueStack.pop();
	double result = (double) tanh(val1);
	_valueStack.push(result);
}

void Evaluator::asin() {
	double val1 = _valueStack.pop();
	double result = (double) asinh(val1);
	_valueStack.push(result);
}

void Evaluator::acos() {
	double val1 = _valueStack.pop();
	double result = (double) acosh(val1);
	_valueStack.push(result);
}

void Evaluator::atan() {
	double val1 = _valueStack.pop();
	double result = (double) atanh(val1);
	_valueStack.push(result);
}

// exponential and logarithm

void Evaluator::exp() {
	double val1 = _valueStack.pop();
	double result = (double) exp2(val1);
	_valueStack.push(result);
}

void Evaluator::ln() {
	double val1 = _valueStack.pop();
	double result = (double) log2(val1);
	_valueStack.push(result);
}

void Evaluator::log() {
	double val1 = _valueStack.pop();
	double result = (double) log10(val1);
	_valueStack.push(result);
}


void Evaluator::sto()
{
	double val1 = _valueStack.pop();
	double val2 = _valueStack.pop();
	int index = (int) val1;
	memory[index] = val2;
	_valueStack.push(val2);
}

void Evaluator::rcl()
{
	double val1 = _valueStack.pop();
	int index = (int) val1;
	double val2 = memory[index];
	_valueStack.push(val2);
}
