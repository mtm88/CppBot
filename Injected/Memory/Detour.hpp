#pragma once
#include "MemoryOperation.hpp"

class Detour :public MemoryOperation
{
public:	
	Detour(int target_func, int hook_func)
		: MemoryOperation((byte*)target_func)
		, hook{ hook_func }
	{
		DWORD old_protection_;
		VirtualProtect(target, 6, PAGE_EXECUTE_READWRITE, &old_protection_);

		new_bytes.push_back(0x68);				// push (the address provided through hook)
		new_bytes.resize(5);
		*(int*)(new_bytes.data() + 1) = hook;	// dirty hack
		new_bytes.push_back(0xc3);				// return

		original_bytes.resize(6);
		memcpy(original_bytes.data(), target, 6);

		Apply();
	}

	~Detour() 
	{		
		Restore();		
	}

	void Apply()
	{
		memcpy(target, new_bytes.data(), 6);
	}

	void Restore()
	{
		memcpy(target, original_bytes.data(), 6);
	}

private:
	int hook{};
};