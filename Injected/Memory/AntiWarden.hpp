#pragma once

int wardenModuleAddress;

int WardenScanDetour(int ptr, int address, int len);

int SMSG_WARDEN_DATA_HandlerDetour(int a1, uint16 opcode, int a3, int pDataStore)
{
	if (opcode == SMSG_WARDEN_DATA)
	{
		if (*(int*)0x00D31A48 != 0)
		{
			int vtable = *(int*)(*(int*)0x00D31A4C);
			int wardenMain = *(int*)(vtable + 8);
			int wardenModule = wardenMain - 0x4099;

			if ((wardenModule & 0xFF) == 0 && wardenModuleAddress != wardenModule)
			{
				wardenModuleAddress = wardenModule;
				printf("\nPatching warden module located at: %X\n", wardenModule);
				
				//doesn't bypass LUA_STR_CHECK and MPQ_CHECK !
				//--------------------------------------------
				DWORD oldPFlags;
				//MODULE_CHECK - tested
				VirtualProtect((void*)(wardenModule + 0x309F), 1, 0x40, &oldPFlags);
				*(byte*)(wardenModule + 0x309F) = 0xE9;
				//DRIVER_CHECK
				VirtualProtect((void*)(wardenModule + 0x33D1), 1, 0x40, &oldPFlags);
				*(byte*)(wardenModule + 0x33D1) = 0;
				//PAGE_CHECK_A, PAGE_CHECK_B - not tested (added thanks to prospectingemu)
				VirtualProtect((void*)(wardenModule + 0x12AD), 1, 0x40, &oldPFlags);
				*(byte*)(wardenModule + 0x12AD) = 0;

				auto det = g_memops["WardenScanDetour"];
				if (det != nullptr)
					delete det;

				g_memops["WardenScanDetour"] = new Detour((wardenModule + 0x2A7F), (int)WardenScanDetour);
			}
		}
	}

	//---------------- return to the original function ----------------
	auto det = g_memops["WardenDataHandler"];
	det->Restore();
	int res = ((int(__cdecl*)(int, uint16, int, int))det->target)(a1, opcode, a3, pDataStore);
	det->Apply();
	return res;
}

int WardenScanDetour(int buffer, int to_compare, int len)
{
	std::map<int, byte> old_bytes_map{};

	for (auto& mem_op : g_memops)
	{
		auto memop = mem_op.second;
		for (int i = 0; i != memop->original_bytes.size(); ++i)
			old_bytes_map[(int)(memop->target) + i] = memop->original_bytes[i];
	}
	
	for (int i = 0; i != len; ++i)	
		if (old_bytes_map.find(to_compare + i) == old_bytes_map.end()) 
			*(byte*)(buffer + i) = *(byte*)(to_compare + i);		
		else 
			//bypass MEM_CHECK for detoured functions
			*(byte*)(buffer + i) = old_bytes_map[to_compare + i];
	return 0;
}