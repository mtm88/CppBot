#pragma once

#include<algorithm>
#include "Object.hpp"
#include "Aura.hpp"

class Unit :public Object
{
public:
	std::vector<Aura*> auras;
	bool auras_readed;

	Unit(int pointer) :auras{}, auras_readed{}, Object(pointer) {}

	~Unit()
	{
		for (auto& aura : auras)
			delete aura;
	}

	//--------------- auras ---------------
	void  ReadAuras()
	{
		if (auras_readed)
			return;

		int idx = addr + 0xC50;
		int count = *(int*)(addr + 0xDD0);
		if (count == -1)
		{
			idx = *(int*)(addr + 0xC58);
			count = *(int*)(addr + 0xC54);
		}
		for (int i = 0; i < count; ++i)
		{
			Aura* aura{ new Aura(idx + 24 * i) };

			if (aura->IsValid()
				&& aura->name != ""
				&& std::none_of(std::begin(auras), std::end(auras), 
					[&](Aura* a) { return strcmp(a->name, aura->name) == 0; }))
			{
				auras.push_back(aura);
			}
		}
		auras_readed = true;
		return;
	}

	bool HasAura(const char* aura)
	{
		ReadAuras();
		return std::any_of(std::begin(auras), std::end(auras), [&](Aura* a) { return strcmp(a->name, aura) == 0; });
	}

	bool AuraNameContains(const char* part)
	{
		ReadAuras();
		return std::any_of(std::begin(auras), std::end(auras),
			[&](Aura* a) { return std::string(a->name).find(part) != std::string::npos; });
	}

	bool HasAnyAura(std::initializer_list<std::string> auraNames)
	{
		ReadAuras();
		return std::any_of(std::begin(auras), std::end(auras),
			[&](Aura* a) {return std::find(auraNames.begin(), auraNames.end(), a->name) != auraNames.end();});
	}

	Aura* GetAuraByName(char* name)
	{
		ReadAuras();
		auto aura = std::find_if(std::begin(auras), std::end(auras), 
			[&](Aura* a) { return strcmp(a->name, name) == 0; });
		if (aura != std::end(auras))
			return *aura;
		return nullptr;
	}

	//--------------- use group ---------------
	bool CanUse(const char* name, bool castedTimeCheck = true)
	{
		return Spell(name).CanUse(Guid(), castedTimeCheck);
	}

	bool Use(const char* name)
	{
		return Spell(name).Use(Guid());
	}

	bool UseFirst(std::initializer_list<std::string> names)
	{
		for (auto name : names)
		{
			if (Use(name.c_str()))
				return true;
		}
		return false;
	}

	//--------------- buff/debuff group ---------------
	bool Buff(const char* buff)
	{
		return HasAura(buff) ? false : Use(buff);
	}

	bool Buff(std::initializer_list<std::string> buffs)
	{
		if (HasAnyAura(buffs))
			return false;

		return UseFirst(buffs);
	}

	bool Debuff(const char* buff)
	{
		return CanAttack() ? Buff(buff) : false;
	}

	bool DebuffRanged(const char* buff)
	{
		if (!HasAura(buff)) return ShootRanged(buff);

		return false;
	}

	//--------------- shoot group ---------------
	bool CanShoot(const char* spell, bool castedTimeCheck = true)
	{
		return CanAttack() && CanUse(spell, castedTimeCheck);
	}

	bool Shoot(const char* spell)
	{
		return CanAttack() && Use(spell);
	}

	bool Shoot(std::initializer_list<std::string> spells)
	{
		return CanAttack() && UseFirst(spells);
	}

	bool ShootMelee(const char* spell) //ShootMeleeCone
	{
		return (IsFacingMelee() && Shoot(spell));
	}

	bool ShootMelee(std::initializer_list<std::string> spells)
	{
		return (IsFacingMelee() && Shoot(spells));
	}

	bool ShootRanged(const char* spell) //ShootRangedCone
	{
		return (IsFacingRanged() && Shoot(spell));
	}

	bool ShootRanged(std::initializer_list<std::string> spells)
	{
		return (IsFacingRanged() && Shoot(spells));
	}
	//---------------------------------------------

	bool InSpellRange(const char* spellName)
	{
		Spell spell = Spell(spellName);
		return spell.InRange(Guid());
	}

	bool IsAutoAttacking() const { return addr ? *(unsigned int*)(addr + 0xA20) != 0 : false; }

	CreatureType GetCreatureType() const
	{
		return  addr ? ((CreatureType(__thiscall*)(int))0x0071F300)(addr) : CREATURE_TYPE_NOT_SPECIFIED;
	}

	uint64 TargetGuid() const
	{
		return GetDescriptor<uint64>(UNIT_FIELD_TARGET);
	}	

	Unit* Target() const
	{
		return (Unit*)objects[TargetGuid()];
	}

	int Health() const
	{
		return GetDescriptor<int>(UNIT_FIELD_HEALTH);
	}

	int MaxHealth() const
	{
		return GetDescriptor<int>(UNIT_FIELD_MAXHEALTH);
	}

	const double HealthPercent() const
	{
		return (Health() / (double)MaxHealth()) * 100;
	}

	bool IsDead() const
	{
		return Health() <= 0 || HasFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
	}

	bool IsGhost() { return Health() == 1; }

	uint64 SummonedBy() const
	{
		return GetDescriptor<uint64>(UNIT_FIELD_SUMMONEDBY);
	}

	bool FeignDeathed() const
	{
		return HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
	}

	byte* DisplayPower() const
	{
		return (byte*)GetPointerDescriptor(UNIT_FIELD_POWER1);
	}

	byte* UnitBytes0() const
	{
		return (byte*)GetPointerDescriptor(UNIT_FIELD_BYTES_0);
	}

	byte* UnitBytes1() const
	{
		(byte*)GetPointerDescriptor(UNIT_FIELD_BYTES_1);
	}

	byte* UnitBytes2() const
	{
		return (byte*)GetPointerDescriptor(UNIT_FIELD_BYTES_2);
	}

	int Race() const { return UnitBytes0()[0]; }			//in fact these guys are byte long
	int Class() const { return UnitBytes0()[1]; }
	int Gender() const { return UnitBytes0()[2]; }
	int PowerType() const { return UnitBytes0()[3]; }

	bool IsLootable() const
	{
		return HasFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
	}

	bool IsTapped() const
	{
		return HasFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_TAPPED);
	}

	bool IsTappedByMe() const
	{
		return HasFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_TAPPED_BY_PLAYER);
	}

	bool IsInCombat() const
	{
		return HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
	}

	bool IsLooting() const
	{
		return HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOOTING);
	}

	int GetPowerByType(Powers power) const
	{
		switch (power)
		{
		default:
		case POWER_MANA:
			return GetDescriptor<int>(UNIT_FIELD_POWER1);
		case POWER_RAGE:
			return GetDescriptor<int>(UNIT_FIELD_POWER2);
		case POWER_ENERGY:
			return GetDescriptor<int>(UNIT_FIELD_POWER3);
		case POWER_FOCUS:
			return GetDescriptor<int>(UNIT_FIELD_POWER4);
		case POWER_HAPPINESS:
			return GetDescriptor<int>(UNIT_FIELD_POWER5);
		case POWER_RUNE:
		case POWER_RUNIC_POWER:
			return GetDescriptor<int>(UNIT_FIELD_POWER7);
		}
	}

	int GetMaxPowerByType(Powers power) const
	{
		switch (power)
		{
		default:
		case POWER_MANA:
			return GetDescriptor<int>(UNIT_FIELD_MAXPOWER1);
		case POWER_RAGE:
			return GetDescriptor<int>(UNIT_FIELD_MAXPOWER2);
		case POWER_ENERGY:
			return GetDescriptor<int>(UNIT_FIELD_MAXPOWER3);
		case POWER_FOCUS:
			return GetDescriptor<int>(UNIT_FIELD_MAXPOWER4);
		case POWER_HAPPINESS:
			return GetDescriptor<int>(UNIT_FIELD_MAXPOWER5);
		case POWER_RUNE:
		case POWER_RUNIC_POWER:
			return GetDescriptor<int>(UNIT_FIELD_MAXPOWER7);
		}
	}

	int Power() const { return GetPowerByType((Powers)PowerType()); }

	int MaxPower() const { return GetMaxPowerByType((Powers)PowerType()); }

	float PowerPercent() const { return (Power() / (float)MaxPower()) * 100; }

	bool CanAttack(const Unit* other) const
	{
		if (!other || other->addr == 0)
			return false;

		if (!other->IsPlayer() && other->IsDead())
			return false;

		return  addr ? ((bool(__thiscall*)(int, int))0x00729740)(addr, other->addr) : false;
	}

	bool CanAttack() const
	{
		return CanAttack(&Unit(GetLocalPlayer()));
	}

	void UpdateDisplayInfo(bool update = true)
	{
		if (addr)
			((void(__thiscall*)(int, bool))0x0073E410)(addr, update);
	}

	int MountDisplayId() const
	{
		return GetDescriptor<int>(UNIT_FIELD_MOUNTDISPLAYID);
	}

	bool Mounted() const { return MountDisplayId() != 0; }

	int MovementFlags() const
	{
		return *(int*)(*(int*)(addr + 0xD8) + 0x44);
	}

	void SetMovementFlags(int flags)
	{
		*(int*)(*(int*)(addr + 0xD8) + 0x44) = flags;
	}

	bool IsMoving() const
	{
		return (MovementFlags() & (int)MOVEMENTFLAG_MASK_MOVING) != 0;
	}	

	uint64 PetGuid() const
	{
		return GetDescriptor<uint64>(UNIT_FIELD_CHARM)?
			GetDescriptor<uint64>(UNIT_FIELD_CHARM) : GetDescriptor<uint64>(UNIT_FIELD_SUMMON);
	}

	Unit* Pet() const { return (Unit*)objects[PetGuid()]; }

	bool HavePet() const
	{
		return Pet() && !Pet()->IsDead();
	}

	bool IsTargetingMe() const
	{
		return GetLocalPlayerGuid() == TargetGuid();
	}

	bool IsTargetingMeOrPet() const
	{
		return IsTargetingMe() || TargetGuid() == (Unit(GetLocalPlayer())).PetGuid();
	}
	

	int ThreatPercentFor(Unit* otherUnit) const
	{
		if (!addr || !otherUnit || !otherUnit->addr)
			return 0;

		uint64 guid = otherUnit->Guid();
		int tStatus = 0;
		int tPercent = 0;
		int tRawPercent = 0;
		int tValue = 0;

		((void(__thiscall*)(int, uint64&, int&, int&, int&, int&))0x007374C0)(addr, guid, tStatus, tPercent, tRawPercent, tValue);

		return tPercent;    // 0..100   or 255 for securely tanking
	}

	int ThreatPercent() const { return ThreatPercentFor(&Unit(GetLocalPlayer())); }

	bool IsThreating(Unit* other) const { return ThreatPercentFor(other) > 0; }

	bool IsThreatingMe() const { return ThreatPercent() > 0; }

	bool IsThreatingMeOrPet() const { return IsThreatingMe() || IsThreating(Unit(GetLocalPlayer()).Pet()); }

	struct ByDistance
	{
		bool operator ()(Unit* a, Unit* b)
		{
			return a->Distance() < b->Distance();
		}
	};

	struct ByDistanceToPet
	{
		bool operator ()(Unit* a, Unit* b)
		{
			return a->DistanceTo(Unit(GetLocalPlayer()).Pet()) < b->DistanceTo(Unit(GetLocalPlayer()).Pet());
		}
	};

	struct ByThreat
	{
		bool operator ()(Unit* a, Unit* b)
		{
			return a->ThreatPercent() < b->ThreatPercent();
		}
	};

	int ChannelledId() const { return addr ? *(int*)(addr + 0xA80) : 0; }

	int CastingId() const { return addr ? *(int*)(addr + 0xA6C) : 0; }

	bool IsCasting() const { return CastingId() != 0; }

	bool IsChannelling() const { return ChannelledId() != 0; }

	bool IsCastingOrChannelling() const { return IsCasting() || IsChannelling(); }

	bool IsFalling() const
	{
		if ((MovementFlags() & MOVEMENTFLAG_FALLING) == 0)
			return false;
		return (MovementFlags() & MOVEMENTFLAG_ROOT) == 0;
	}
};