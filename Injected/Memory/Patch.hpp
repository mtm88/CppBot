#pragma once
#include "MemoryOperation.hpp"

class Patch :public MemoryOperation
{
public:
	Patch(int target_addr, std::initializer_list<byte> bytes) : MemoryOperation((byte*)target_addr, bytes)
	{
		DWORD old_protection_;
		VirtualProtect(target, bytes.size(), PAGE_EXECUTE_READWRITE, &old_protection_);		

		original_bytes.resize(bytes.size());
		memcpy(original_bytes.data(), target, bytes.size());

		Apply();
	}

	~Patch()
	{		
		Restore();
	}

	void Apply()
	{		
		memcpy(target, new_bytes.data(), new_bytes.size());
	}

	void Restore()
	{
		memcpy(target, original_bytes.data(), original_bytes.size());
	}
};