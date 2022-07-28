// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Paladyn.h"

#include "KrzyweKarty/Gameplay/KKGameMode.h"
#include "KrzyweKarty/Map/KKMap.h"

AZakon_Paladyn::AZakon_Paladyn()
{

}

bool AZakon_Paladyn::ActiveAbility(AKKCharacter* TargetCharacter)
{
	if(GetMana() < GetFirstAbilityManaCost())
		return false;
	
	for(AKKCharacter* Character : GetAffectedCharacters())
	{
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
		if(!Map->MoveLeft(TargetCharacter) || !Map->MoveRight(TargetCharacter))
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
	TArray AffectedTiles = {
		OwnedTileID - 4,
		OwnedTileID + 4,
		(OwnedTileID % 4 != 3) ? OwnedTileID + 4 + 1 : -1, //to the right
		(OwnedTileID % 4 != 0) ? OwnedTileID + 4 - 1 : -1, //to the left
		OwnedTileID + 4 + 4
	};

	if(AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
	{
		TArray<AKKCharacter*> Characters = GameMode->GetMap()->Characters;
		for(int32 TileID : AffectedTiles)
		{
			if(Characters.IsValidIndex(TileID))
			{
				if(Characters[TileID] != nullptr)
				{
					AffectedCharacters.Add(Characters[TileID]);
				}
			}
		}
	}

	return AffectedCharacters;
}

