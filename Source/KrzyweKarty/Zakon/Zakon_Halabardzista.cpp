// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Halabardzista.h"
#include "KrzyweKarty/Map/KKMap.h"

// bool AZakon_Halabardzista::ActiveAbility()
// {
// 	if (GetMana() < GetFirstAbilityManaCost())
// 		return false;
//
// 	CharacterStats.MaxAttackRange = 3;
//
// 	// if (!DefaultAttackConditions(ReverseState, EAT_ActiveAbility))
// 	// 	return false;
// 	//
// 	// switch (GetDistanceTo(ReverseState))
// 	// {
// 	// case 3:
// 	// 	DealDamage(ReverseState, 7);
// 	// 	break;
// 	// case 2:
// 	// 	DealDamage(ReverseState, 9);
// 	// 	break;
// 	// case 1:
// 	// 	DealDamage(ReverseState, 9);
// 	// 	break;
// 	// default:
// 	// 	break;
// 	// }
//
// 	CharacterStats.MaxAttackRange = CharacterDataAsset->CharacterStats.MaxAttackRange;
//
// 	DecreaseManaForFirstAbility();
// 	return true;
// }


int32 AZakon_Halabardzista::DefineDamageAmount(AKKCharacter* TargetCharacter)
{
	switch (GetDistanceTo(TargetCharacter))
	{
		case 1: return GetStrength();
		case 2: return  6;
		default: return 0;
	}
}

TArray<FDirection> AZakon_Halabardzista::GetAffectedTiles_Implementation(int32 Index)
{
	return {
		{1, 0},
		{2, 0},
		{3, 0}
	};
}
