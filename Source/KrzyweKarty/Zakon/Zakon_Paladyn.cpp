// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Paladyn.h"

#include "KrzyweKarty/Gameplay/KKGameMode.h"
#include "KrzyweKarty/Map/KKMap.h"

bool AZakon_Paladyn::ActiveAbility(AKKCharacter* TargetCharacter)
{
	if(GetMana() < GetFirstAbilityManaCost())
		return false;
	
	// for(AKKCharacter* Character : {}) //todo: FIX using GetAffectedTiles(1)
	// {
	// 	if(!Character)
	// 		continue;
	//
	// 	
	// 	if(IsInTheSameTeam(Character))
	// 	{
	// 		Character->IncreaseHealth(4);
	// 	}
	// 	else
	// 	{
	// 		DealDamage(Character, 10);
	// 	}
	// }

	DecreaseManaForFirstAbility();
	return true;
}

bool AZakon_Paladyn::ActiveAbility2(AKKCharacter* TargetCharacter)
{
	if(GetMana() < GetSecondAbilityManaCost())
		return false;

	if(AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
	{
		AKKMap* Map = GameMode->GetMap();
		if(!Map->MoveCharacter(TargetCharacter, EMD_Left) || !Map->MoveCharacter(TargetCharacter, EMD_Right)) // GetAffectedTiles(2);
		{
			DealDamage(TargetCharacter, 24);
			DecreaseManaForSecondAbility();
			return true;
		}
	}
	
	return false;
}

bool AZakon_Paladyn::CanBeAttacked(EAttackType AttackType)
{
	if(GetHealth() < 15 && (AttackType == EAT_ActiveAbility || AttackType == EAT_PassiveAbility))
		return false;

	return Super::CanBeAttacked(AttackType);
}

TArray<FDirection> AZakon_Paladyn::GetAffectedTiles(int32 Index)
{
	switch (Index)
	{
	case 1:
		return {
			{-1, 0},
			{1, -1},
			{1, 0},
			{1, 1},
			{2,0}
		};
		/*
			 #
			###
			 O
			 #
		*/
	case 2:
		return {
			{1, -1},
			{1, 1}
		};
	default:
		return {};
	}

	
}

