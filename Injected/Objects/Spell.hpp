#pragma once

#include "..\WowStuff\WowFuncs.hpp"
#include "..\WowStuff\WowClientDB.hpp"

std::map<int, timer> casted;
std::vector<int> known_spells;
bool known_spells_updated = false;

void inline UpdateKnownSpells()
{
	for (int i = 0; i < *(int*)0x00BE8D9C; ++i)
	{
		int spellId = *(int*)(0x00BE5D88 + (i * 4));
		known_spells.push_back(spellId);
		known_spells_updated = true;
	}
}

int SpellIdByName(const char*);

class Spell
{
public:
	int id;
	SpellRec record;
	const char* name;

	Spell() :id{}, name{} {}

	Spell(int arg_id) :id{ arg_id }
	{			
		//WowClient_GetLocalizedRow(0x00AD49D0, id, record);
		db_tables[ClientDB::Spell].WowClient_GetLocalizedRow(id, record);

		name = record.m_name_lang;
	}

	Spell(const char* arg_name) :name{ arg_name }, id{ SpellIdByName(arg_name) }
	{	
		db_tables[ClientDB::Spell].WowClient_GetLocalizedRow(id, record);
	}	

	bool Cast(uint64 at_guid, int itemId = 0, bool isTrade = 0)
	{
		if (!at_guid)
			return false;

		if (casted.find(id) != casted.end())
			if (casted[id].elapsedTime() < 0.2)
				return false;

		casted[id] = timer();		

		return ((bool(__cdecl*)(int, int, uint64, bool))0x0080DA40)(id, itemId, at_guid, isTrade);
	}	

	bool InRange(uint64 from_guid)
	{
		if (!from_guid)
			return false;		

		bool inRange = false;

		bool validUnit = ((bool(__cdecl*)(int, SpellRec&, uint64, bool&, int, int))(0x00809610))(GetLocalPlayer(), record, from_guid, inRange, 0, 0);
		return validUnit && inRange;
	}		

	float CoolDown()
	{
		int start, duration, unk0;
		start = duration = unk0 = 0;
		bool isReady = false;

		((bool(__cdecl*)(int, bool, int&, int&, bool&, int&))(0x00809000))(id, /*isPet*/ false, duration, start, isReady, unk0);

		int result = start + duration - PerformanceCount();
		return isReady ? (result > 0 ? result / 1000.0f : 0) : FLT_MAX;
	}

	bool IsReady() { return CoolDown() <= 0; }	

	bool NoContraintsToCastSpell(int pet_addr = 0)
	{
		int addr = pet_addr ? pet_addr : GetLocalPlayer();
		return ((bool(__cdecl*)(int, SpellRec&, uint64*, int, bool))0x0080B5D0)(addr, record, /*ptrToSomeGuid*/nullptr, /*itemCashe*/0, /*unk*/false);
	}

	bool EnoughPowerToCastSpell(int pet_addr = 0)
	{
		int addr = pet_addr ? pet_addr : GetLocalPlayer();
		return ((bool(__cdecl*)(int, SpellRec&))0x008017E0)(addr, record);
	}

	bool IsUsable()
	{
		return NoContraintsToCastSpell() && EnoughPowerToCastSpell();
	}

	bool HasRange(int pet_addr = 0)
	{
		int addr = pet_addr ? pet_addr : GetLocalPlayer();
		float min{}, max{};
		((bool(__cdecl*)(int, SpellRec&, float&, float&, int))0x00801650)(addr, record, min, max, 0);
		return min > 0 || max > 0;
	}

	bool CanUse(uint64 at_guid, bool castedTimeCheck = true)
	{
		if (!at_guid)
			return false;

		if (castedTimeCheck
			&& (casted.find(id) != casted.end())
			&& casted[id].elapsedTime() < 0.2)
			return false;

		return (std::find(known_spells.begin(), known_spells.end(), id) != known_spells.end())
			&& IsUsable() 
			&& IsReady()
			&& (!HasRange() || InRange(at_guid));		// && unit->InLos());  put this guy in Unit class
	}	

	bool Use(uint64 at_guid)
	{
		return CanUse(at_guid) ? Cast(at_guid) : false;
	}
};

inline int SpellIdByName(const char* name)
{
	int rank;
	return ((int(__cdecl*)(const char*, int*))0x00540200)(name, &rank);
}

inline bool CastSpellOnPlayer(char* spellName, int itemId = 0)
{
	int id = SpellIdByName(spellName);

	if (casted.find(id) != casted.end())
		if (casted[id].elapsedTime() < 0.2)
			return false;

	casted[id] = timer();

	return ((bool(__cdecl*)(int, int, uint64))0x0080DA80)(id, itemId, 0);
	return true;
}