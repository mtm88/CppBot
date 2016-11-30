#pragma once

class Detour
{
public:
	
	byte* target;
	int hook;
	byte* original_bytes;
	byte* new_bytes;
	bool is_applied;

	Detour(int target_func, int hook_func)
		: target{ (byte*)target_func }
		, hook{ hook_func }
		, original_bytes{ new byte[6] }
		, new_bytes{ new byte[6] }
		, is_applied{ false }
	{
		LoadFunctionMemory();
		Apply();
	}

	~Detour() 
	{
		if (is_applied)
		{
			Restore();
			delete original_bytes;
			delete new_bytes;
		}
	}

	bool Apply()
	{
		if (is_applied)
			return false;
		
		memcpy(target, new_bytes, 6);
		
		is_applied = true;
		return true;
	}

	bool Restore()
	{
		if (!is_applied)
			return false;

		memcpy(target, original_bytes, 6);
		
		is_applied = false;
		return true;
	}

//private:
	void LoadFunctionMemory()
	{
		DWORD old_protection_;
		VirtualProtect(target, 6, PAGE_EXECUTE_READWRITE, &old_protection_);

		*new_bytes = 0x68;  // push (address provided through hookFunc)
		*(int*)(new_bytes + 1) = hook;
		new_bytes[5] = 0xc3;  // return
		
		memcpy(original_bytes, target, 6);
	}	
};