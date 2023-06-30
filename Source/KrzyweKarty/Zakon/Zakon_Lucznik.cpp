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

bool AZakon_Lucznik::ActiveAbility(AKKCharacter* TargetCharacter) //Trojstrzal
{
	if (GetMana() < GetFirstAbilityManaCost())
		return false;
	
	for(AKKCharacter* Character : GetAffectedCharacters())
	{
		if(MinAttackConditions(Character, EAT_ActiveAbility))
		{
			DealDamage(Character, 5);
		}
	}
	
	DecreaseManaForFirstAbility();
	
	return true;

}

TArray<FDirection> AZakon_Lucznik::GetPossibleAttackTiles()
{
	TArray<FDirection> DefaultAttackTiles = Super::GetPossibleAttackTiles();
	DefaultAttackTiles.Append({
	{2,0}, {3,0},
	{-2,0}, {-3,0},
	{0,2}, {0,3},
	{0,-2}, {0,-3}
	});
	
	return DefaultAttackTiles;
}

TArray<AKKCharacter*> AZakon_Lucznik::GetAffectedCharacters()
{
	TArray<AKKCharacter*> AffectedCharacters;
	
	if(AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
	{
		AffectedCharacters = GameMode->GetMap()->GetCharactersAtTiles(this,
		{
			{2, -1},
			{2, 0},
			{2, 1}
		});
	}
	
	return AffectedCharacters;
}
