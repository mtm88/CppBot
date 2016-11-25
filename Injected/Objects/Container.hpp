#pragma once
#include "Item.hpp"
class Container :public Item
{
public:
	Container(int pointer) :Item(pointer) {}
};