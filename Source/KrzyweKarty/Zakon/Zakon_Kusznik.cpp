// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Kusznik.h"

bool AZakon_Kusznik::DefaultAttack(AKKCharacter* TargetCharacter)
{
	if(SecondAbilityInUse)
	{
		SetStrength(GetStrength() + 2);
		if(Super::DefaultAttack(TargetCharacter))
		{
			SetStrength(GetDefaultStrength());
			SecondAbilityInUse = false;
			return true;
		}
		return false;
	}
	
	if(Super::DefaultAttack(TargetCharacter))
	{
		IncreaseHealth(2);
		return true;
	}

	return false;
}

bool AZakon_Kusznik::ActiveAbility(AKKCharacter* TargetCharacter)
{
	if(GetMana() < GetFirstAbilityManaCost())
		return false;

	if(!DefaultAttackConditions(TargetCharacter, EAT_ActiveAbility))
		return false;

	DealDamage(TargetCharacter, 15);
	TargetCharacter->DecreaseDefence();

	DecreaseManaForFirstAbility();
	return true;
}

bool AZakon_Kusznik::ActiveAbility2(AKKCharacter* TargetCharacter)
{
	if(GetHealth() < GetSecondAbilityManaCost())
		return false;

	IncreaseMana(2);
	SecondAbilityInUse = true;
	DecreaseHealth(GetSecondAbilityManaCost());
	return true;
}

