/*
 * Evaluator.cpp
 *
 *  Created on: 15. stu 2020.
 *      Author: bkumpar
 */

#include <math.h>
#include "Evaluator.h"

Evaluator::Evaluator() {

}

Evaluator::~Evaluator() {
}

void Evaluator::init() {
	_operators[L"+"] = new MathOperator<FunctionPtr>(2, left, &Evaluator::add);
	_operators[L"-"] = new MathOperator<FunctionPtr>(2, left, &Evaluator::sub);
	_operators[L"*"] = new MathOperator<FunctionPtr>(2, left, &Evaluator::mul);
	_operators[L"/"] = new MathOperator<FunctionPtr>(2, left, &Evaluator::div);
	_operators[L"^"] = new MathOperator<FunctionPtr>(2, left, &Evaluator::pwr);
	_operators[L"min"] = new MathOperator<FunctionPtr>(2, left,
			&Evaluator::min);
	_operators[L"max"] = new MathOperator<FunctionPtr>(2, left,
			&Evaluator::max);
	_operators[L"sin"] = new MathOperator<FunctionPtr>(2, left,
			&Evaluator::sin);
	_operators[L"cos"] = new MathOperator<FunctionPtr>(2, left,
			&Evaluator::cos);
}
void Evaluator::pushToOutput(std::wstring token) {
	_output += token + L" ";
}

void Evaluator::processFunction(std::wstring token) {
	_operatorStack.push(token);
}

void Evaluator::procesLeftParenthesis(std::wstring token) {
	_operatorStack.push(token);
}
void Evaluator::procesRightParenthesis() {
	while (_operatorStack.top() != L"(") {
		std::wstring token = _operatorStack.pop();
		pushToOutput(token);
		if (_operatorStack.top() == L"(") {
			_operatorStack.drop();
		}
	}
}

void Evaluator::processNumber(std::wstring token) {
	double number = stod(token);
	_valueStack.push(number);
	pushToOutput(token);
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
	std::wstring operatorOnStack = _operatorStack.top();
	MathOperatorPtr operatorOnStackPtr = _operators[operatorOnStack];
	MathOperatorPtr thisOperatorPtr = _operators[thisOperator];
	return (*operatorOnStackPtr) > (*thisOperatorPtr);
}

bool Evaluator::operatorOnStackHasEqualPrecedenceAndLeftAssociativity(
		std::wstring thisOperator) {
	std::wstring operatorOnStack = _operatorStack.top();
	MathOperatorPtr operatorOnStackPtr = _operators[operatorOnStack];
	MathOperatorPtr thisOperatorPtr = _operators[thisOperator];
	Associativity operatorAssociativity = thisOperatorPtr->associativity();
	return (*operatorOnStackPtr) == (*thisOperatorPtr)
			&& (operatorAssociativity == left);
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
	MathOperatorPtr operatorPtr = _operators[op];
	FunctionPtr function = operatorPtr->function();
	(this->*function)();
}

void Evaluator::parse(std::wstring expression) {
	std::wstring token = L"";
	bool tokenIsNumber = false;
	for (uint i = 0; i < expression.length(); i++) {
		wchar_t ch = expression[i];
		if (isDigit(ch)) {
			tokenIsNumber = true;
			token += ch;
		} else if (ch == ',') {
			if (token != L"") {
				if (tokenIsNumber) {
					processNumber(token);
				} else {
					processFunction(token);
				}
			}

		} else if (ch == '(') {
			procesLeftParenthesis(token);

		} else if (ch == ')') {
			procesRightParenthesis();
		} else if (isOperator(ch)) {
			if (token != L"" && tokenIsNumber) {
				processNumber(token);
				token = L"";
			}
		} else {
			token += ch;
		}

	}
}

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
	double result = (double) sinl(val1);
	_valueStack.push(result);
}

void Evaluator::cos() {
	double val1 = _valueStack.pop();
	double result = (double) cosl(val1);
	_valueStack.push(result);
}
