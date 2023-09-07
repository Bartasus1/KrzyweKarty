// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Lucznik.h"
#include "KrzyweKarty/Map/KKMap.h"

FAttackResultInfo AZakon_Lucznik::DefaultAttack(AKKCharacter* TargetCharacter)
{
	FAttackResultInfo AttackResult = Super::DefaultAttack(TargetCharacter);

	if (AttackResult.AttackResultEnum == EAttackResult::AttackConfirmed)
	{
		AttacksCounter++;
	}

	if (AttacksCounter >= 2)
	{
		TargetCharacter->DecreaseDefence(); //Strzaly Rozrywajace
		AttacksCounter = 0;
	}

	return AttackResult;
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
