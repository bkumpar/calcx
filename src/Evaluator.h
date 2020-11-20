/*
 * Evaluator.h
 *
 *  Created on: 15. stu 2020.
 *      Author: bkumpar
 */
#ifndef EVALUATOR_H_
#define EVALUATOR_H_

#include <map>
#include <string>
#include "SimpleStack.h"
#include "MathOperator.h"

class Evaluator;

// declaration of pointer to void method of class Operator
typedef void (Evaluator::*FunctionPtr)(void);
typedef MathOperator<FunctionPtr> MathOperatorFn;
//typedef MathOperatorFn *MathOperatorPtr;
typedef std::map<std::wstring, MathOperatorFn> OperatorsMap;

class Evaluator {
public:
	Evaluator();
	virtual ~Evaluator();
	void init();
	void parse(std::wstring expression);
	double result();
	std::wstring rpn();
private:
	SimpleStack<double> _valueStack;
	SimpleStack<std::wstring> _operatorStack;
	OperatorsMap _operators;
	std::wstring _output;
	bool isDigit(wchar_t ch);
	bool isOperator(wchar_t ch);

	void pushToOutput(std::wstring token);
	void processFunction(std::wstring token);
	void procesLeftParenthesis();
	void procesRightParenthesis();
	void processNumber(std::wstring number);
	void processOperator(std::wstring op);

	bool operatorStackIsNotEmpty();
	bool operatorOnStackHasGreaterPrecedence(std::wstring thisOperator);
	bool operatorOnStackHasEqualPrecedenceAndLeftAssociativity(	std::wstring thisOperator);
	bool operatorOnStackIsNotLeftParentheses();
	void finish();
	void calc(std::wstring op);

	void add();
	void sub();
	void mul();
	void div();
	void pwr();
	void sqrt();
	void min();
	void max();
	void sin();
	void cos();
	void pi();

}
;

#endif /* EVALUATOR_H_ */
