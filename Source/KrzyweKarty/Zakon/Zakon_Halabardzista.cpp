// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Halabardzista.h"

AZakon_Halabardzista::AZakon_Halabardzista()
{
}

bool AZakon_Halabardzista::ActiveAbility(AKKCharacter* TargetCharacter)
{
	if (GetMana() < GetAbilityManaCost())
		return false;

	if (TargetCharacter == nullptr || !IsInLineWith(TargetCharacter))
		return false;

	if (TargetCharacter->CanBeAttacked(EAT_ActiveAbility) && !IsFromSameFraction(TargetCharacter))
	{
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

		DecreaseManaForAbility();
		return true;
	}

	return false;
}

int32 AZakon_Halabardzista::GetStrengthAtDistance(int32 Distance)
{
	if (Distance == 2)
	{
		return 6;
	}

	return GetStrength();
}
