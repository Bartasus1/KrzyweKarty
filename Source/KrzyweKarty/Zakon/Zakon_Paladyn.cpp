// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Paladyn.h"

#include "KrzyweKarty/Gameplay/KKGameMode.h"
#include "KrzyweKarty/Map/KKMap.h"

bool AZakon_Paladyn::ActiveAbility(AKKCharacter* TargetCharacter)
{
	if(GetMana() < GetFirstAbilityManaCost())
		return false;
	
	for(AKKCharacter* Character : GetAffectedCharacters())
	{
		if(!Character)
			continue;

		
		if(IsInTheSameTeam(Character))
		{
			Character->IncreaseHealth(4);
		}
		else
		{
			DealDamage(Character, 10);
		}
	}

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
		if(!Map->MoveCharacter(TargetCharacter, EMD_Left) || !Map->MoveCharacter(TargetCharacter, EMD_Right))
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



TArray<AKKCharacter*> AZakon_Paladyn::GetAffectedCharacters()
{
	TArray<AKKCharacter*> AffectedCharacters;
	
	if(AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
	{
		AffectedCharacters = GameMode->GetMap()->GetCharactersAtTiles(this,
		{
			{-1, 0},
			{1, -1},
			{1, 0},
			{1, 1},
			{2,0}
		});
	}

	return AffectedCharacters;
}

