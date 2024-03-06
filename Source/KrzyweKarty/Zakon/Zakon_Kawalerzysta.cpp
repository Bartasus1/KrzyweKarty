// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Kawalerzysta.h"
#include "KrzyweKarty/Map/KKMap.h"


bool AZakon_Kawalerzysta::CanUseAbility_Implementation(uint8 Index)
{
	if(Super::CanUseAbility_Implementation(Index) == false)
	{
		return false;
	}
	
	TArray<FMapCell*> MapCells = GetAbilityMapCells(Index);
	
	switch (Index)
	{
	case 1:
		return MapCells.Num() == 2 && MapCells[0]->Character != nullptr && MapCells[1] == nullptr;
	case 2:
		return MapCells.Num() == 1 && MapCells[0]->Character != nullptr;
	default:
		return false;
	}
}

TArray<FDirection> AZakon_Kawalerzysta::GetPossibleMoveTiles()
{
	return{
		{-1, 0},
		{1, 0},
		{2, 0},
		{0, -1},
		{0, 1}
	};
}

TArray<FMapCell*> AZakon_Kawalerzysta::GetAbilityMapCells(uint8 Index)
{
	TArray<FMapCell*> MapCells;
	
	switch (Index)
	{
	case 1:
		if(FMapCell* EnemyTile = GetMap()->GetCellByDirection(this, {1, 0}))
		{
			MapCells.Add(EnemyTile);
		}
		if(FMapCell* DestinationTile = GetMap()->GetCellByDirection(this, {2, 0}))
		{
			MapCells.Add(DestinationTile);
		}
		break;
	case 2:
		if(FMapCell* EnemyTile = GetMap()->GetCellByDirection(this, {-1, 0}))
		{
			MapCells.Add(EnemyTile);
		}
		break;
	default:
		break;
	}

	return MapCells;
}

