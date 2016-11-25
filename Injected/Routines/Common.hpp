#pragma once
#include "..\OM.hpp"

void PaladinLeveling();
void HunterLeveling();

bool gottarget;
bool havepet;
std::vector<Unit*> enemies;
timer looted_time;

void Common()
{
	if (!me)
		return;

	gottarget = me->CanAttack(target);
	pet = me->Pet();
	havepet = me->HavePet();
	enemies = find_all(units, [&](Unit* u) { return u->CanAttack(); });

	if (me->IsDead()
		|| me->IsGhost()
		|| me->HasAura("Shadowmeld")
		|| me->FeignDeathed()
		|| me->HasAnyAura({ "Food", "Drink", "Graccu's Mince Meat Fruitcake", "Dim Sum" })
		|| me->Name() == "UNKNOWN"
		|| me->Mounted()
		//|| ShadowmeldFeignDeathTimer.Millisec() < 3500     // todo: a implement trough event spell sent
		|| (me->Mounted() && me->Class() == CLASS_PALADIN && me->Buff("Crusader Aura")))
		return;

	if (me->Class() == CLASS_PALADIN)
		PaladinLeveling();
	else if (me->Class() == CLASS_HUNTER)
		HunterLeveling();
}

bool Loot()
{
	if (me->PartyMemberGuids().size() > 0 || looted_time.elapsedTime() < 0.25)
		return false;

	auto to_loot = std::find_if(units.begin(), units.end(), [&](Unit* u) { return u->IsLootable() && u->Distance() < 6; });

	if (to_loot != std::end(units))
	{
		(*to_loot)->Interact();
		looted_time.start();
	}
	return false;
}

std::vector<Unit*> EnemiesInXXXRange(float xxx)
{
	return find_all(units, [&](Unit* e)
	{ 
		return e->Distance() < xxx
			&& (e->IsTargetingMeOrPet() || e->IsThreatingMe()); 
	});		
}

bool DotUnits(std::string dotSpell)
{
	auto to_dot = std::find_if(enemies.begin(), enemies.end(), [&](Unit* e)
	{
		return e != target
			//&& e.CreatureType != CreatureType.Critter
			&& e->IsThreatingMe()
			&& e->DebuffRanged(dotSpell.c_str());
			
	});

	return to_dot != std::end(enemies);
}

//Gothcha!!!
#include "PaladinLeveling.hpp"
#include "HunterLeveling.hpp"