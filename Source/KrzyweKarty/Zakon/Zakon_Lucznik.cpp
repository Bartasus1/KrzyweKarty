// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Lucznik.h"

#include "KrzyweKarty/Gameplay/KKGameMode.h"
#include "KrzyweKarty/Map/KKMap.h"

bool AZakon_Lucznik::DefaultAttack(AKKCharacter* TargetCharacter)
{
	const bool SuccessfulAttack = Super::DefaultAttack(TargetCharacter);

	if (SuccessfulAttack)
	{
		AttacksCounter++;
	}

	if (AttacksCounter >= 2)
	{
		TargetCharacter->DecreaseDefence(); //Strzaly Rozrywajace
		AttacksCounter = 0;
	}

	return SuccessfulAttack;
}

// bool AZakon_Lucznik::ActiveAbility() //Trojstrzal
// {
// 	if (GetMana() < GetFirstAbilityManaCost())
// 		return false;
// 	
// 	// for(AKKCharacter* Character : {}) //todo: FIX using GetAffectedTiles()
// 	// {
// 	// 	if(MinAttackConditions(Character, EAT_ActiveAbility))
// 	// 	{
// 	// 		DealDamage(Character, 5);
// 	// 	}
// 	// }
// 	
// 	DecreaseManaForFirstAbility();
// 	
// 	return true;
//
// }


TArray<FDirection> AZakon_Lucznik::GetAffectedTiles_Implementation(int32 Index)
{
	return {
		{2, -1},
		{2, 0},
		{2, 1}
	};
	/* 
		###
		 
		 O 
	*/
}
