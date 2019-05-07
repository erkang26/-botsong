// FileName: Stack.cpp
//
// Desc:
//
// Created by token.tong at 2019-05-07 15:31:37
#include "Stack.h"

Stack::Stack()
{
}

Stack::~Stack()
{
}

void Stack::push( void* p )
{
	_stack.push(p);
}
void* Stack::pop()
{
	if ( empty() )
	{
		return NULL;
	}

	void* p = _stack.top();
	_stack.pop();

	return p;
}
void* Stack::top()
{
	if ( empty() )
	{
		return NULL;
	}

	return _stack.top();
}
bool Stack::empty()
{
	return _stack.empty();
}
size_t Stack::size()
{
	return _stack.size();
}
