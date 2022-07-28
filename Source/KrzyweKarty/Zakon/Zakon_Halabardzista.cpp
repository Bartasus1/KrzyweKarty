// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Halabardzista.h"

AZakon_Halabardzista::AZakon_Halabardzista()
{
}

bool AZakon_Halabardzista::ActiveAbility(AKKCharacter* TargetCharacter)
{
	if (GetMana() < GetFirstAbilityManaCost())
		return false;

	CharacterStats.MaxAttackRange = 3;

	if (!DefaultAttackConditions(TargetCharacter, EAT_ActiveAbility))
		return false;
	
	switch (GetDistanceTo(TargetCharacter))
	{
	case 3:
		DealDamage(TargetCharacter, 7);
		break;
	case 2:
		DealDamage(TargetCharacter, 9);
		break;
	case 1:
		DealDamage(TargetCharacter, 9);
		break;
	default:
		break;
	}

	CharacterStats.MaxAttackRange = CharacterDataAsset->CharacterStats.MaxAttackRange;

	DecreaseManaForFirstAbility();
	return true;
}

int32 AZakon_Halabardzista::GetStrengthAtDistance(int32 Distance)
{
	if (Distance == 2)
	{
		return 6;
	}

	return GetStrength();
}
