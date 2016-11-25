#pragma once
#include "Player.hpp"

timer last_petattack;

class LocalPlayer :public Player
{
public:
	LocalPlayer(int pointer) :Player(pointer) {}	

	std::vector<uint64> PartyMemberGuids()
	{
		std::vector<uint64> party_guids{};

		for (int i = 0; i < 5; i++)
		{
			uint64 guid = *(uint64*)(0x00BD1948 + 4 * i);

			if (guid)
				party_guids.push_back(guid);
		}
		return party_guids;
	}
	
	bool IsInInstance() const
	{
		int cur_map_id = *(int*)0x00AB63BC;
		int map = db_tables[ClientDB::Map].GetRowPtr(cur_map_id);
		if (!map)
			return false;
		int instance_type = *(int*)(map + 2 * 4);
		return instance_type != 0;
	}

	bool IsAutoshootingH() const { return addr? *(int*)(addr + 0x1858) == 6 : false; }

	int GetAutoshootingH() const { return addr ? *(int*)(addr + 0x1858) : 0; }

	bool StopAttack()
	{		
		if (addr && (IsAutoAttacking() || IsAutoshootingH()))
		{
			((bool(__thiscall*)(int))0x006E1660)(addr);
			return true;
		}
		return false;
	}

	bool StopCasting()
	{
		if (addr && IsCasting())
		{
			((void(__stdcall*)())0x00809A80)();
			return true;
		}
		return false;
	}

	bool SendPetAction(Unit* unit, int action)
	{
		if (!unit || !HavePet())
			return false;

		int _action = action;
		uint64 guid = unit->Guid();

		((void(__cdecl*)(int&, uint64&, int))0x005D4210)(action, guid, 1);
		return true;
	}

	bool PetAttack(Unit* unit)
	{
		if (HavePet() && Pet()->Target() == unit)
			return false;
		if (last_petattack.elapsedTime() < 0.1)
			return false;
		last_petattack.start();
		int action = 0x07000002;        //petattack     
		return SendPetAction(unit, action);
	}	

	bool IsSpellPending() const { return *(uint32*)0x00AF5250 != 0x00AF5250; }
	
};