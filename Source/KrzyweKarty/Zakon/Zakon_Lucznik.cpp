// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Lucznik.h"

#include "KrzyweKarty/Gameplay/KKGameMode.h"
#include "KrzyweKarty/Map/KKMap.h"

AZakon_Lucznik::AZakon_Lucznik()
{
}

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
	if (GetMana() < GetAbilityManaCost())
		return false;
	
	for(AKKCharacter* Character : GetAffectedCharacters())
	{
		if(Character->CanBeAttacked(EAT_ActiveAbility) && !IsInTheSameTeam(Character))
		{
			DealDamage(Character, 5);
		}
	}
	
	DecreaseManaForFirstAbility();
	
	return true;

}

TArray<AKKCharacter*> AZakon_Lucznik::GetAffectedCharacters()
{
	TArray<AKKCharacter*> AffectedCharacters;
	
	if(AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
	{
		AKKMap* Map = GameMode->GetMap();
		int32 MapSize = Map->GetMapSize();

		const int32 MyRow = OwnedTileID / MapSize;
		const int32 MyColumn = OwnedTileID % MapSize;

		const int32 TargetRow = FMath::Clamp(MyRow + 2, 0, MapSize);

		for (int32 TargetColumn = MyColumn - 1; TargetColumn <= MyColumn + 1; TargetColumn++) // 3 collumn attack
		{
			const int32 TargetTileID = FMath::Clamp((TargetRow * MapSize) + TargetColumn, 0, 19);

			if (TargetTileID / MapSize == TargetRow) //check if target is on the targetRow (might changed if we are close to map boundaries
			{
				if(AKKCharacter* Character = Map->Characters[TargetTileID])
				{
					AffectedCharacters.Add(Character);
				}
			}
		}
	}
	
	return AffectedCharacters;
}
