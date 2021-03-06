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
	if (GetMana() < GetFirstAbilityManaCost())
		return false;

	if (IsInTheSameTeam(TargetCharacter))
	{
		TargetCharacter->IncreaseHealth(3);
		TargetCharacter->IncreaseMana(4);
	}

	DecreaseManaForFirstAbility();
	return true;
}

bool AZakon_Kaplan::ActiveAbility2(AKKCharacter* TargetCharacter)
{
	if (GetMana() < GetSecondAbilityManaCost())
		return false;

	if (DefaultAttackConditions(TargetCharacter, EAT_ActiveAbility))
	{
		DealDamage(TargetCharacter, 17);
		
		DecreaseManaForSecondAbility();
		return true;
	}

	return false;
}
