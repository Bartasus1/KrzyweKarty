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
	if (GetMana() < GetAbilityManaCost() || !HasAuthority())
		return false;

	if (AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
	{
		AKKMap* Map = GameMode->GetMap();
		int32 MapSize = Map->GetMapSize();

		int32 MyRow = OwnedTileID / MapSize;
		int32 MyCollumn = OwnedTileID % MapSize;

		int32 TargetRow = FMath::Clamp(MyRow + 2, 0, MapSize);

		for (int32 i = MyCollumn - 1; i <= MyCollumn + 1; i++) // 3 collumn attack
		{
			int32 TargetTileID = FMath::Clamp((TargetRow * MapSize) + i, 0, 19);

			if (TargetTileID / MapSize == TargetRow) //check if target is on the targetRow (might changed if we are close to map boundaries) 
			{
				AKKCharacter* Character = Map->Characters[TargetTileID];
				
				if(Character->CanBeAttacked(EAT_ActiveAbility) && !IsFromSameFraction(Character))
				{
					DealDamage(Character, 5);
				}
			}
		}

		DecreaseManaForAbility();
		return true;
	}

	return false;
}
