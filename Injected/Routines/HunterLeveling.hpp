#pragma once
#include "Common.hpp"

void HunterLeveling()
{
	bool safeshot = false;

	if (gottarget &&
		(	!havepet
			|| target->IsPlayer() 
			|| std::string(target->Name()).find("Training Dummy") != std::string::npos 
			|| me->IsInInstance())
		)
	{
		safeshot = true;
	}
	
	if (gottarget
		&& havepet
		&& target->IsThreatingMeOrPet()
		&& target->ThreatPercent() < 70)
	{
		safeshot = true;
	}

	if (false) {}

	else if ((!gottarget || !target->CanShoot("Auto Shot", false) || !safeshot)
		&& me->IsAutoshootingH()
		&& me->StopAttack())
	{
	}

	else if (gottarget
		&& !me->IsMoving()
		&& target->CanShoot("Auto Shot")
		&& target->FaceRanged())
	{
	}

	else if (havepet
		//  if i dont have target or have target and it threatens us then do PetAttack
		//  if i have target that dont threatens us then skip
		&& !(gottarget && !target->IsThreatingMeOrPet())
		&& (me->PetAttack(min_by(
							find_all(enemies, [&](Unit* e)
							{
								return e->IsTargetingMe()
									&& (!me->IsInInstance()
										|| e->DistanceTo(pet) < 7
										|| e->Distance() < 25);
							}), Unit::ByDistance())
						)
		|| !me->IsInInstance()
		&& me->PetAttack(max_by(find_all(enemies, [&](Unit* e)
							{return e->IsThreatingMe();}), Unit::ByThreat())
						)
			)
		)
	{
	}

	else if (havepet
		&& gottarget
		&& target != pet->Target()
		&& !target->IsThreatingMeOrPet()
		&& me->PetAttack(target))
	{
	}

	else if (havepet
		&& pet->Target()
		&& me->PetAttack(min_by(
							find_all(enemies, [&](Unit* e)
							{
								return e->IsThreatingMeOrPet()
									&& (!me->IsInInstance()
										|| e->DistanceTo(pet) < 7
										|| e->Distance() < 25);
							}), Unit::ByDistanceToPet())
						)
		)
	{
	}

	else if (!target
		&& havepet
		&& pet->Target()
		&& (pet->Target())->CanAttack()
		&& (pet->Target())->Select())
	{
	}

	else if (gottarget
		&& target->CanShoot("Kill Shot")
		&& (me->HasAura("Aspect of the Viper") || !me->AuraNameContains("Aspect"))
		&& me->Buff({ "Aspect of the Dragonhawk", "Aspect of the Hawk" }))
	{
	}

	else if (gottarget
		&& target->ShootRanged("Kill Shot"))
	{
	}

	else if (havepet
		&& gottarget
		&& pet->InMeleeTo(target)
		&& pet->Use("Bestial Wrath"))
	{
	}

	else if (safeshot
		&& target->ShootRanged("Chimera Shot"))
	{
	}

	else if (safeshot
		&& target->ShootRanged("Aimed Shot"))
	{
	}

	else if (havepet
		&& gottarget
		&& pet->InMeleeTo(target)
		&& pet->Use("Kill Command"))         //TODO replace pet.inmeleeto with canshoot kill command
	{
	}

	else if (havepet
		&& pet->HealthPercent() < 95
		&& (!pet->HasAura("Mend Pet") || (pet->GetAuraByName("Mend Pet"))->RemainingMilliSecs() < 1500)
		&& me->IsCastingOrChannelling()
		&& pet->CanUse("Mend Pet"))
	{
		me->StopCasting();
	}

	else if (havepet
		&& pet->HealthPercent() < 95
		&& (!pet->HasAura("Mend Pet")
			|| (pet->GetAuraByName("Mend Pet"))->RemainingMilliSecs() < 1500)
		&& pet->Use("Mend Pet"))
	{
	}

	else if ((me->HasAura("Aspect of the Viper") || !me->AuraNameContains("Aspect"))
		&& me->PowerPercent() >= 90
		&& me->IsInCombat()
		&& me->Buff({ "Aspect of the Dragonhawk", "Aspect of the Hawk" }))
	{
	}

	else if (me->PowerPercent() <= 25
		&& me->Buff("Aspect of the Viper"))
	{
	}

	else if (EnemiesInXXXRange(5).size() > 1
		//&& !me.IsMoving
		//&& me.ManaPercent > 40.0
		&& (me->Use("Explosive Trap")
			|| !me->IsInInstance() && me->Use("Snake Trap")))
	{
	}

	else if (safeshot
		&& target->CanShoot("Auto Shot")
		&& me->Use("Rapid Fire"))
	{
	}

	else if (me->Race() == RACE_TROLL
		&& safeshot
		&& target->CanShoot("Auto Shot")
		&& me->Use("Berserking"))
	{
	}

	else if (me->HasAura("Aspect of the Cheetah")
		&& me->IsInCombat()
		&& me->Buff({ "Aspect of the Dragonhawk", "Aspect of the Hawk" }))
	{
	}

	else if (gottarget
		&& target->IsMoving()
		&& target->IsTargetingMe()
		//&& !target->IsSlowed()																					yes, have to do this too
		//&& !target.IsCCed
		&& target->ShootRanged({ "Concussive Shot", "Wing Clip" }))
	{
	}

	else if (safeshot
		&& !me->IsMoving()
		&& EnemiesInXXXRange(40).size() > 1	
		&& EnemiesInXXXRange(40)[0]->Shoot("Multi-Shot"))
	{
	}

	else if (safeshot
		&& target->DebuffRanged("Serpent Sting"))
	{
	}

	else if (safeshot
		&& target->CanShoot("Chimera Shot")
		&& (me->HasAura("Aspect of the Viper") || !me->AuraNameContains("Aspect"))
		&& me->Buff({"Aspect of the Dragonhawk", "Aspect of the Hawk"}))
	{
	}

	else if (DotUnits("Serpent Sting"))																		
	{
	}

	else if (safeshot
		&& target->ShootRanged("Arcane Shot"))
	{
	}

	else if (safeshot
		&& target->IsFacingRanged()
		&& !me->IsFalling()
		&& !me->IsAutoshootingH()
		&& target->InSpellRange("Auto Shot"))
		//&& me->Use("Auto Shot"))
	{
		CastSpellOnPlayer("auto shot");
	}

	else if (gottarget
		&& me->Power() >= 7
		&& !target->HasAura("Hunter's Mark")
		&& target->Shoot("Hunter's Mark"))
	{
	}

	/*else if (gottarget
		&& me->IsAutoshootingH()												todo ----------->>>>>
		&& UseTrinket())
	{
	}*/

	else if (safeshot
		&& target->IsFacingRanged()
		&& !me->IsFalling()
		&& !me->IsMoving()
		&& !me->IsCastingOrChannelling()
		&& target->Shoot("Steady Shot"))
	{
	}

	else if (gottarget
		&& !me->IsAutoAttacking()
		&& target->Distance() < 5
		&& target->Health() > 0
		&& target->ShootMelee("Auto Attack"))
	{		
	}

	else if (gottarget
		&& target->IsTargetingMe()
		&& target->InMelee()
		&& !me->IsMoving()
		&& target->FaceMelee())
	{
	}

	else if (gottarget
		&& target->InMelee()
		&& !me->IsSpellPending()
		&& target->ShootMelee("Raptor Strike"))
	{
	}

	else if (Loot()) {}

	else if (me->Buff("Trueshot Aura")) {}
}