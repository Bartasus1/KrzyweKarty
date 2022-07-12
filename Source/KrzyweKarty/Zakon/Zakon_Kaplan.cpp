// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Kaplan.h"

AZakon_Kaplan::AZakon_Kaplan()
{
}

bool AZakon_Kaplan::DefaultAttack(AKKCharacter* TargetCharacter)
{
	if (Super::DefaultAttack(TargetCharacter))
	{
		IncreaseMana(3);
		return true;
	}

	return false;
}

bool AZakon_Kaplan::ActiveAbility(AKKCharacter* TargetCharacter)
{
	if (GetMana() < GetAbilityManaCost())
		return false;

	if (IsFromSameFraction(TargetCharacter))
	{
		TargetCharacter->IncreaseHealth(3);
		TargetCharacter->IncreaseMana(4);
	}
	else
	{
		IncreaseHealth(3);
		IncreaseMana(4);
	}

	DecreaseManaForAbility();
	return true;
}

bool AZakon_Kaplan::ActiveAbility2(AKKCharacter* TargetCharacter)
{
	if (GetMana() < GetAbilityManaCost() || TargetCharacter == nullptr)
		return false;

	if (TargetCharacter->CanBeAttacked(EAT_ActiveAbility) && !IsFromSameFraction(TargetCharacter))
	{
		DealDamage(TargetCharacter, 17);
		
		DecreaseManaForAbility(1);
		return true;
	}

	return false;
}
