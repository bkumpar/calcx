/*
 * Operator.cpp
 *
 *  Created on: 16. stu 2020.
 *      Author: bkumpar
 */

#include <string>

typedef enum {
	left, right
} Associativity;

template<typename T>
class MathOperator {
public:
	MathOperator() :
			_precedence(0), _associativity(left), _function(NULL) {
	}

	MathOperator(int precedence, Associativity associativity, T function) :
			_precedence(precedence), _associativity(associativity), _function(
					function) {
	}

	bool operator>(MathOperator rightOperator) {
		return this->_precedence > rightOperator._precedence;
	}

	bool operator<(MathOperator rightOperator) {
		return this->_precedence < rightOperator._precedence;
	}

	bool operator==(MathOperator rightOperator) {
		return this->_precedence == rightOperator._precedence;
	}

	Associativity associativity() {
		return _associativity;
	}

	T function() {
		return _function;
	}

private:
	int _precedence;
	Associativity _associativity;
	T _function;

};
