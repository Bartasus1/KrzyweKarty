// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Lucznik.h"
#include "KrzyweKarty/Map/KKMap.h"

FAttackResultInfo AZakon_Lucznik::DefaultAttack(AKKCharacter* TargetCharacter)
{
	FAttackResultInfo AttackResult = Super::DefaultAttack(TargetCharacter);

	if (AttackResult.AttackStatus == EAttackResult::AttackConfirmed)
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

void AZakon_Lucznik::PerformAbility_Implementation(int32 Index)
{
	const TArray<FDirection> FinalAffectedTiles = Execute_GetFinalAffectedTiles(this, Index);

	for(AKKCharacter* Character : GetMap()->GetCharactersByDirection(this, FinalAffectedTiles, CSP_EnemyCharactersOnly))
	{
		FAttackResultInfo AttackResultInfo;
		Character->ApplyDamageToSelf(5, AttackResultInfo);
	}
}


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
