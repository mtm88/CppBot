#pragma once

#include "Spell.hpp"

class Aura
{
public:
	int pointer;
	AuraRec entry;
	Spell spell;
	int id;
	const char* name;

	Aura(int addr) :pointer{ addr }
	{
		entry = *(AuraRec*)addr;
		id = entry.AuraId;
		spell = Spell(id);
		name = spell.name;
	}

	int RemainingMilliSecs()
	{
		int endTime = entry.EndTime;
		return endTime == 0 ? 0 : endTime - PerformanceCount();
	}

	bool IsValid()
	{
		return id != 0 && RemainingMilliSecs() >= 0;
	}

	uint64 CasterGuid()
	{
		return entry.CreatorGuid;
	}
};
