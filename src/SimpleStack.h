/*
 * SimpleStack.h
 *
 *  Created on: 16. stu 2020.
 *      Author: bkumpar
 */
#ifndef SIMPLESTACK_H_
#define SIMPLESTACK_H_

#include <string>
#include <list>
#include <exception>


template <typename T>
class SimpleStack
{
public:
	SimpleStack()
	{
		init(L"SimpleStack");
	}
	SimpleStack(std::wstring name)
	{
		init(name);
	}
	virtual ~SimpleStack()
	{

	}

	void clear()
	{
		_container.clear();
	}

	int size()
	{
		return _container.size();
	}

	bool isEmpty()
	{
		return _container.empty();
	}

	bool isNotEmpty()
	{
		return !isEmpty();
	}

	void drop()
	{
		_container.pop_back();
	}

	T pop()
	{
		T &topElement = _container.back();
		T copyOfTopElement = topElement;
		_container.pop_back();
		return copyOfTopElement;
	}

	T top()
	{
		T &topElement = _container.back();
		return topElement;
	}

	void push(T element)
	{
		_container.push_back(element);
	}


private:
	void init(std::wstring name)
	{
		_name = name;
	}
	std::wstring _name;
	std::list<T> _container;

};

#endif /* SIMPLESTACK_H_ */
