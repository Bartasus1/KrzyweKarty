// Fill out your copyright notice in the Description page of Project Settings.

#include "KKMap.h"
#include "KKTile.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"

// Sets default values
AKKMap::AKKMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	MapMesh = CreateDefaultSubobject<UStaticMeshComponent>("MapMesh");
	MapMesh->SetRelativeScale3D(FVector(1.072922, 0.949018, 1));
}

bool AKKMap::AddCharacterToMap(AKKCharacter* Character, int32 TileID)
{
	FMapCell* Destination = GetCellAtIndex(TileID);
	
	if (Destination && Destination->Character == nullptr && !Character->IsCharacterOnMap())
	{
		AssignCharacterToTile(Character, Destination);
		return true;
	}

	return false;
}

bool AKKMap::MoveCharacter(AKKCharacter* Character, int32 TileID)
{
	FMapCell* Destination = GetCellAtIndex(TileID);

	GetCellAtIndex(Character->OwnedTileID)->Character = nullptr;
	AssignCharacterToTile(Character, Destination);

	return true;
	
}

// bool AKKMap::MoveCharacter(AKKCharacter* Character, EMovementDirection MovementDirection)
// {
// 	FMapCell* Destination = GetCellAtIndex(Character->OwnedTileID + MovementDirection);
// 	
// 	if(Destination && Destination->Character == nullptr && Character->IsCharacterOnMap())
// 	{
// 		GetCellAtIndex(Character->OwnedTileID)->Character = nullptr;
// 		AssignCharacterToTile(Character, Destination);
//
// 		return true;
// 	}
//
// 	return false;
// }

TArray<AKKCharacter*> AKKMap::GetCharactersAtTiles(AKKCharacter* Character, TArray<FDirection> Tiles)
{
	check(Character);
	
	TArray<AKKCharacter*> FoundCharacters;

	const int32 X = GetX(Character->OwnedTileID);
	const int32 Y = GetY(Character->OwnedTileID);

	const int32 Direction = (Character->OwningPlayer->PlayerID == 1) ? 1 : -1;
	
	for(const auto& Tile : Tiles)
	{
		const int32 NextX = X + (Direction * Tile.X);
		const int32 NextY = Y + (Direction * Tile.Y);
		
		if(IsValidIndex(NextX, NextY) && MapArray[NextX].MapRows[NextY].Character != nullptr)
		{
			FoundCharacters.Add(MapArray[NextX].MapRows[NextY].Character);
		}
	
	}

	return FoundCharacters;
}

TArray<AKKTile*> AKKMap::GetTilesByDirection(AKKCharacter* Character, TArray<FDirection> Tiles, bool bFilterOutCharacters)
{
	check(Character);
	
	TArray<AKKTile*> FoundTiles;

	const int32 X = GetX(Character->OwnedTileID);
	const int32 Y = GetY(Character->OwnedTileID);

	const int32 Direction = Character->Direction;
	
	for(const FDirection& Tile : Tiles)
	{
		const int32 NextX = X + (Direction * Tile.X);
		const int32 NextY = Y + (Direction * Tile.Y);
		
		if(IsValidIndex(NextX, NextY))
		{
			if(bFilterOutCharacters && MapArray[NextX].MapRows[NextY].Character != nullptr)
				continue;
			
			FoundTiles.Add(MapArray[NextX].MapRows[NextY].Tile);
		}
	}

	return FoundTiles;
}

TArray<AKKTile*> AKKMap::GetTilesForSpawn(AKKCharacter* Character, TArray<int32> TilesID)
{
	check(Character);
	
	TArray<AKKTile*> FoundTiles;
	
	for(const auto& TileID : TilesID)
	{
		FMapCell* MapCell = GetCellAtIndex((Character->Direction == 1) ? TileID : 19 - TileID);

		if(MapCell->Character == nullptr)
		{
			FoundTiles.Add(MapCell->Tile);
		}
	}
	

	return FoundTiles;
}

TArray<AKKTile*> AKKMap::GetTilesWithCharacters(AKKCharacter* Character, TArray<FDirection> Tiles)
{
	check(Character);
	
	TArray<AKKTile*> FoundTiles;

	const int32 X = GetX(Character->OwnedTileID);
	const int32 Y = GetY(Character->OwnedTileID);

	const int32 Direction = Character->Direction;
	
	for(const FDirection& Tile : Tiles)
	{
		const int32 NextX = X + (Direction * Tile.X);
		const int32 NextY = Y + (Direction * Tile.Y);
		
		if(IsValidIndex(NextX, NextY) && MapArray[NextX].MapRows[NextY].Character != nullptr)
		{
			FoundTiles.Add(MapArray[NextX].MapRows[NextY].Tile);
		}
	}

	return FoundTiles;
}

TArray<AKKTile*> AKKMap::GetTiles(TArray<int32> TilesID)
{
	TArray<AKKTile*> FoundTiles;
	
	for(const auto& TileID : TilesID)
    {
        FoundTiles.Add(GetCellAtIndex(TileID)->Tile);
    }
	
	return FoundTiles;
}

TArray<AKKTile*> AKKMap::GetAllTilesOnMap()
{
	TArray<AKKTile*> AllTiles;
	for(int i = 0; i < TotalMapSize; i++)
	{
		if(const FMapCell* MapCell = GetCellAtIndex(i))
		{
			AllTiles.Add(MapCell->Tile);
		}
	}

	return AllTiles;
}

TArray<AKKCharacter*> AKKMap::GetAllCharactersOnMap()
{
	TArray<AKKCharacter*> AllCharacters;
	for(int i = 0; i < TotalMapSize; i++)
	{
		if(const FMapCell* MapCell = GetCellAtIndex(i))
		{
			if(MapCell->Character != nullptr)
			{
				AllCharacters.Add(MapCell->Character);
			}
		}
	}

	return AllCharacters;
}

TArray<AKKCharacter*> AKKMap::GetAllyCharactersOnMap(AKKCharacter* Character)
{
	return GetAllCharactersOnMap().FilterByPredicate([=](AKKCharacter* AnotherCharacter) -> bool
	{
		return Character->IsInTheSameTeam(AnotherCharacter);
	});
}

TArray<AKKCharacter*> AKKMap::GetEnemyCharactersOnMap(AKKCharacter* Character)
{
	return GetAllCharactersOnMap().FilterByPredicate([=](AKKCharacter* AnotherCharacter) -> bool
	{
		return !(Character->IsInTheSameTeam(AnotherCharacter));
	});
}

void AKKMap::ClearTilesHighlights()
{
	for(int i = 0; i < 20; i++)
	{
		GetCellAtIndex(i)->Tile->SetTileColor(None);
	}
}

AKKTile* AKKMap::GetTileAtIndex(int32 TileID)
{
	return GetCellAtIndex(TileID) ? GetCellAtIndex(TileID)->Tile : nullptr;
}

AKKCharacter* AKKMap::GetCharacterAtIndex(int32 TileID)
{
	return GetCellAtIndex(TileID) ? GetCellAtIndex(TileID)->Character : nullptr;
}

void AKKMap::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetupMap();
	}
}

void AKKMap::SetupMap()
{
	check(TileClass);
	
	constexpr uint16 SpacingX = 125;
	constexpr uint16 SpacingY = 100;
	
	for (int i = 0; i < 5; i++)
	{
		MapArray.Add(FMapRow());
		
		for (int j = 0; j < 4; j++)
		{
			FVector TileLocation = StartLocation;
			TileLocation.X += i * SpacingX;
			TileLocation.Y += j * SpacingY;
			
			AKKTile* Tile = GetWorld()->SpawnActor<AKKTile>(TileClass, TileLocation, GetActorRotation());
			Tile->TileID = GetID(i , j);
			Tile->TextRenderComponent->SetText(FText::FromString(FString::FromInt(Tile->TileID)));

			MapArray[i].MapRows.Add({Tile, nullptr});
		}
	}
}


void AKKMap::AssignCharacterToTile(AKKCharacter* Character, FMapCell* MapCell)
{
	MapCell->Character = Character;
	
	Character->OwnedTileID = MapCell->Tile->TileID;
	Character->SetActorLocation(MapCell->Tile->GetActorLocation());
}

FMapCell* AKKMap::GetCellAtIndex(int32 TileID)
{
	const int32 X = GetX(TileID);
	const int32 Y = GetY(TileID);
	
	if(IsValidIndex(X, Y))
	{
		return &MapArray[X].MapRows[Y];
	}
	
	return nullptr;
}

void AKKMap::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AKKMap, MapArray);
}

