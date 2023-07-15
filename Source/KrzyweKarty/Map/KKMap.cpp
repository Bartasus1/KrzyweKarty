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

TArray<AKKCharacter*> AKKMap::GetCharactersAtTiles(AKKCharacter* Character, TArray<FDirection> RelativeTiles, ECharacterSelectionPolicy CharacterSelectionPolicy)
{
	check(Character);
	
	TArray<AKKCharacter*> FoundCharacters;

	const int32 X = GetX(Character->OwnedTileID);
	const int32 Y = GetY(Character->OwnedTileID);

	const int32 Direction = (Character->OwningPlayer->PlayerID == 1) ? 1 : -1;
	
	for(const auto& Tile : RelativeTiles)
	{
		const int32 NextX = X + (Direction * Tile.X);
		const int32 NextY = Y + (Direction * Tile.Y);
		
		if(IsValidIndex(NextX, NextY) && MapArray[NextX].MapRows[NextY].Character != nullptr)
		{
			switch (CharacterSelectionPolicy)
			{
			case CSP_AllCharacters:
				FoundCharacters.Add(MapArray[NextX].MapRows[NextY].Character);
				break;
			case CSP_AllyCharactersOnly:
				if(Character->IsInTheSameTeam(MapArray[NextX].MapRows[NextY].Character))
				{
					FoundCharacters.Add(MapArray[NextX].MapRows[NextY].Character);
				}
				break;
			case CSP_EnemyCharactersOnly:
				if(!Character->IsInTheSameTeam(MapArray[NextX].MapRows[NextY].Character))
				{
					FoundCharacters.Add(MapArray[NextX].MapRows[NextY].Character);
				}
				break;
			default:
				break;
			}
		}
	
	}

	return FoundCharacters;
}

TArray<AKKTile*> AKKMap::GetTilesByDirection(AKKCharacter* Character, TArray<FDirection> RelativeTiles, ETileSelectionPolicy TileSelectionPolicy)
{
	TArray<AKKTile*> FoundTiles;

	const int32 X = GetX(Character->OwnedTileID);
	const int32 Y = GetY(Character->OwnedTileID);

	const int32 Direction = Character->Direction;
	
	for(const FDirection& Tile : RelativeTiles)
	{
		const int32 NextX = X + (Direction * Tile.X);
		const int32 NextY = Y + (Direction * Tile.Y);
		
		if(IsValidIndex(NextX, NextY))
		{
			FMapCell MapCell = MapArray[NextX].MapRows[NextY];
			
			switch (TileSelectionPolicy)
			{
			case TSP_None:
				FoundTiles.Add(MapCell.Tile);
				break;
				
			case TSP_NoCharacters:
				if(MapCell.Character == nullptr)
				{
					FoundTiles.Add(MapCell.Tile);
				}
				break;
			case TSP_AllyCharactersOnly:
				if(MapCell.Character != nullptr && Character->IsInTheSameTeam(MapCell.Character))
				{
					FoundTiles.Add(MapCell.Tile);
				}
				break;
			case TSP_EnemyCharactersOnly:
				if(MapCell.Character != nullptr && !Character->IsInTheSameTeam(MapCell.Character))
				{
					FoundTiles.Add(MapCell.Tile);
				}
				break;
			default:
				break;
			}
		}
	}

	return FoundTiles;
}

TArray<AKKTile*> AKKMap::GetTilesForSpawn(AKKCharacter* Character, TArray<int32> TilesID)
{
	check(Character);
	
	TArray<AKKTile*> FoundTiles;
	
	for(int32& TileID : TilesID)
	{
		FMapCell* MapCell = GetCellAtIndex((Character->Direction == 1) ? TileID : 19 - TileID);

		if(MapCell->Character == nullptr)
		{
			FoundTiles.Add(MapCell->Tile);
		}
	}
	
	return FoundTiles;
}

TArray<AKKTile*> AKKMap::GetTiles(TArray<int32> TilesID)
{
	TArray<AKKTile*> FoundTiles;
	
	for(int32& TileID : TilesID)
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
		return (Character != AnotherCharacter) && (Character->IsInTheSameTeam(AnotherCharacter));
	});
}

TArray<AKKCharacter*> AKKMap::GetEnemyCharactersOnMap(AKKCharacter* Character)
{
	return GetAllCharactersOnMap().FilterByPredicate([=](AKKCharacter* AnotherCharacter) -> bool
	{
		return (Character != AnotherCharacter) && (!Character->IsInTheSameTeam(AnotherCharacter));
	});
}

TArray<AKKTile*> AKKMap::CanAttackBase(AKKCharacter* Character, TArray<AKKTile*> InDefaultAttackTiles)
{
	const bool bIsForwardFacing = Character->Direction == 1;
	int32 BaseAttackTiles[2] = {bIsForwardFacing ? 17: 1, bIsForwardFacing ? 18: 2};
	int32 BaseIndex = bIsForwardFacing ? 1 : 0; // if facing forward, character is attacking client, so client base index is 1
	

	if(Character->OwnedTileID == BaseAttackTiles[0] || Character->OwnedTileID == BaseAttackTiles[1])
	{
		InDefaultAttackTiles.Add(BaseArray[BaseIndex].Tile);
	}

	return InDefaultAttackTiles;
}

void AKKMap::ClearTilesHighlights()
{
	for(int i = 0; i < 20; i++)
	{
		GetCellAtIndex(i)->Tile->SetTileColor(None);
	}

	for(auto& BaseTile: BaseArray)
	{
		BaseTile.Tile->SetTileColor(None);
	}
}

AKKTile* AKKMap::GetTileAtIndex(int32 TileID)
{
	if(TileID == INT32_MAX || TileID == INT32_MAX - 1)
	{
		return BaseArray[INT32_MAX - TileID].Tile;
	}
	
	return GetCellAtIndex(TileID) ? GetCellAtIndex(TileID)->Tile : nullptr;
}

AKKCharacter* AKKMap::GetCharacterAtIndex(int32 TileID)
{
	if(TileID == INT32_MAX || TileID == INT32_MAX - 1)
	{
		return BaseArray[INT32_MAX - TileID].Character;
	}
	
	return GetCellAtIndex(TileID) ? GetCellAtIndex(TileID)->Character : nullptr;
}

void AKKMap::RemoveCharacterFromTile(int32 TileID)
{
	if(FMapCell* MapCell = GetCellAtIndex(TileID))
	{
		MapCell->Character = nullptr;
	}
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
	
	
	uint16 SpacingX = 125;
	uint16 SpacingY = 100;
	
	for (int32 i = 0; i < BaseRow; i++)  // 5
	{
		MapArray.Add(FMapRow());
		
		for (int j = 0; j < BaseRow - 1; j++) // 4
		{
			FVector TileLocation = StartLocation;
			TileLocation.X += i * SpacingX;
			TileLocation.Y += j * SpacingY;
			
			AKKTile* Tile = GetWorld()->SpawnActor<AKKTile>(TileClass, TileLocation, GetActorRotation());
			Tile->TileID = GetID(i , j);
			Tile->OnRep_TileID();
			
			MapArray[i].MapRows.Add({Tile, nullptr});
		}
	}
	
	const float BaseLocationX[2] = { -338.f, 338.f};
	const float BaseRotationYaw[2] = { 90.f, 270.f};
	
	for (int32 i = 0; i < 2; i++)
	{
		AKKTile* Tile = GetWorld()->SpawnActor<AKKTile>(TileClass, FVector(BaseLocationX[i], 0.f, 0.f), FRotator(0.f, BaseRotationYaw[i], 0.f));
		Tile->TileID = INT32_MAX - i;
		Tile->OnRep_TileID();

		BaseArray.Add({Tile, nullptr});
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

void AKKMap::SetFractionBase(int32 ID, AKKCharacter* Base)
{
	// ID comes in 1-2 range -> make it 0-1
	ID--;

	AssignCharacterToTile(Base, &BaseArray[ID]);
}


void AKKMap::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AKKMap, MapArray);
	DOREPLIFETIME(AKKMap, BaseArray);
}

