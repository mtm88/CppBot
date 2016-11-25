#pragma once
#include "Common.hpp"

void PaladinLeveling()
{
	if (false) {}

	else if (me->HealthPercent() < 15.0 && me->CanUse("Lay on Hands"))
	{
		me->Use("Lay on Hands");
	}

	else if (gottarget
		&& !target->IsTargetingMe()
		&& target->Shoot("Hand of Reckoning"))
	{
	}

	else if (gottarget
		&& !me->IsAutoAttacking()
		&& target->Distance() < 6
		&& target->ShootMelee("Auto Attack"))
	{
	}

	else if (gottarget
		&& target->IsTargetingMe()
		&& target->Distance() < 7
		&& !target->IsFacingMelee()
		&& target->FaceMelee())
	{
	}

	else if (me->HealthPercent() < 75
		&& me->HasAnyAura({ "The Art of War", "Infusion of Light" })
		&& me->Use("Flash of Light"))
	{
	}

	else if (!me->AuraNameContains("Aura")
		&& me->Buff({ "Retribution Aura", "Devotion Aura" }))
	{
	}

	else if (!me->AuraNameContains("Seal")
		&& enemies.size() > 0
		&& me->Buff({ "Seal of Command", "Seal of Vengeance", "Seal of Righteousness" }))
	{
	}

	else if (gottarget
		&& target->ShootMelee({ "Judgement of Wisdom", "Judgement of Light" }))
	{
	}

	else if (gottarget && target->ShootMelee("Crusader Strike"))
	{
	}

	else if (gottarget
		&& target->Distance() < 15
		&& !me->IsMoving()
		&& me->PowerPercent() > 40
		&& me->Use("Consecration"))
	{
	}

	else if (gottarget
		&& me->HasAura("The Art of War")
		&& target->ShootMelee("Exorcism"))
	{
	}

	else if (gottarget
		&& target->Distance() < 15
		&& me->PowerPercent() > 50
		&& me->Use("Divine Storm"))
	{
	}

	else if (gottarget
		&& target->Distance() < 15
		&& me->PowerPercent() > 50
		&& me->Use("Holy Wrath"))
	{
	}

	else if (!me->AuraNameContains("Blessing")
		&& me->Buff({ "Blessing of Kings", "Blessing of Might" }))
	{
	}

	else if (Loot()) {}
}