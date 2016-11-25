#pragma once
#include "Object.hpp"
class DynamicObject :public Object
{
public:
	DynamicObject(int pointer) :Object(pointer)	{}
};