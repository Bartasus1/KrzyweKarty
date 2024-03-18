// Fill out your copyright notice in the Description page of Project Settings.

#include "KKMap.h"
#include "Fraction.h"
#include "KKTile.h"
#include "KrzyweKarty/TileStatusSettings.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKGameMode.h"
#include "Net/UnrealNetwork.h"

FDirection FDirection::Rotate(const ERotationDirection Rotation) const
{
	switch (Rotation)
	{
	case ERD_Forward:
		return {X , Y};
	case ERD_Right:
		return {Y , X};
	case ERD_Backward:
		return {-X , -Y};
	case ERD_Left:
		return {-Y , -X};
	default:
		return *this;
	}
}

ERotationDirection FDirection::GetDirectionLine() const
{
	if(X != 0) // "vertical"
	{
		return (X > 0) ? ERD_Forward : ERD_Backward;
	}
	else // "horizontal"
	{
		return (Y > 0) ? ERD_Right : ERD_Left;
	}
}


TMap<ERotationDirection, TArray<FDirection>> FDirection::SortDirections(const TArray<FDirection>& Directions)
{
	TMap<ERotationDirection, TArray<FDirection>> SortedDirections;

	for (const FDirection& Direction : Directions)
	{
		SortedDirections.FindOrAdd(Direction.GetDirectionLine()).Add(Direction);
		SortedDirections[Direction.GetDirectionLine()].Sort(); // not optimal but whatever
	}

	return SortedDirections;
}

// Sets default values
AKKMap::AKKMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	MapMesh = CreateDefaultSubobject<UStaticMeshComponent>("MapMesh");
	MapMesh->SetRelativeScale3D(FVector(1.072922, 0.949018, 1));
}

bool AKKMap::AddCharacterToMap(AKKCharacter* Character, uint8 TileID)
{
	FMapCell* Destination = GetCellAtIndex(TileID);
	
	if (Destination && Destination->Character == nullptr && !Character->IsCharacterOnMap())
	{
		AssignCharacterToTile(Character, Destination);
		return true;
	}

	return false;
}

bool AKKMap::MoveCharacter(AKKCharacter* Character, uint8 TileID)
{
	FMapCell* Destination = GetCellAtIndex(TileID);

	if(Destination && Destination->Character == nullptr)
	{
		GetCellAtIndex(Character->OwnedTileID)->Character = nullptr;
        AssignCharacterToTile(Character, Destination);
		
		return true;
	}

	return false;
}

void AKKMap::ShowTilesForAttack_Implementation(AKKCharacter* Character)
{
	TArray<AKKTile*> InitialTiles = GetTilesByDirection(Character, Character->GetPossibleAttackTiles(), TSP_EnemyCharactersOnly, true);

	if(CanAttackBase(Character))
	{
		GetTilesForBaseAttack(Character, InitialTiles);
	}

	for(AKKTile* Tile : InitialTiles)
	{
		Tile->SetTileStatus(UTileStatusSettings::GetDataAsset()->AttackTileStatus);
	}
	
}

TArray<AKKCharacter*> AKKMap::GetCharactersByDirection(AKKCharacter* Character, const TArray<FDirection>& Directions, ECharacterSelectionPolicy CharacterSelectionPolicy, bool bBlockDirectionOnFound)
{
	TArray<AKKCharacter*> FoundCharacters;
	
	for(FMapCell& MapCell : GetCellsByDirection(Character, Directions, bBlockDirectionOnFound))
	{
		if(MapCell.Character == nullptr)
		{
			continue;
		}
		
		switch (CharacterSelectionPolicy)
		{
		case CSP_AllCharacters:
			FoundCharacters.Add(MapCell.Character);
			break;
		case CSP_AllyCharactersOnly:
			if(Character->IsInTheSameTeam(MapCell.Character))
			{
				FoundCharacters.Add(MapCell.Character);
			}
			break;
		case CSP_EnemyCharactersOnly:
			if(!Character->IsInTheSameTeam(MapCell.Character))
			{
				FoundCharacters.Add(MapCell.Character);
			}
			break;
		default:
			break;
		}
	}

	return FoundCharacters;
}

TArray<AKKTile*> AKKMap::GetTilesByDirection(AKKCharacter* Character, const TArray<FDirection>& Directions, ETileSelectionPolicy TileSelectionPolicy, bool bBlockDirectionOnFound)
{
	TArray<AKKTile*> FoundTiles;
	
	for(FMapCell& MapCell : GetCellsByDirection(Character, Directions, bBlockDirectionOnFound)) 
	{
		
		switch (TileSelectionPolicy)
		{
		case TSP_AllTiles:
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
	
	return FoundTiles;
}

TArray<FMapCell> AKKMap::GetCellsByDirection(AKKCharacter* Character, const TArray<FDirection>& Directions, bool bBlockDirectionOnFound)
{
	TArray<FMapCell> FoundCells;

	if(Character)
	{
		if(bBlockDirectionOnFound && !Directions.IsEmpty())
		{
			TMap<ERotationDirection, TArray<FDirection>> SortedDirections = FDirection::SortDirections(Directions);
			for (uint8 i = ERD_Forward; i < ERD_Max; i++) //for each Direction Line
			{
				for (const FDirection& DirectionLine : SortedDirections[static_cast<ERotationDirection>(i)])
				{
					if(const FMapCell* MapCell = GetCellByDirection(Character, DirectionLine))
					{
						FoundCells.Add(*MapCell);
						if(MapCell->Character)
						{
							break;
						}
					}
				}
			}
		}
		else
		{
			for(const FDirection& Direction : Directions)
            {
				if(const FMapCell* MapCell = GetCellByDirection(Character, Direction))
				{
					FoundCells.Add(*MapCell);
				}
            }
		}
	}

	return FoundCells;
}

TArray<AKKTile*> AKKMap::GetTilesForSpawn(AKKCharacter* Character, TArray<uint8> TilesID)
{
	TArray<AKKTile*> FoundTiles;

	if(Character)
	{
		for(uint8& TileID : TilesID)
		{
			FMapCell* MapCell = GetCellAtIndex((Character->Direction == 1) ? TileID : 19 - TileID);

			if(MapCell->Character == nullptr)
			{
				FoundTiles.Add(MapCell->Tile);
			}
		}
	}
	
	return FoundTiles;
}

TArray<AKKTile*> AKKMap::GetTiles(TArray<uint8> TilesID)
{
	TArray<AKKTile*> FoundTiles;
	
	for(uint8& TileID : TilesID)
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

TArray<AKKCharacter*> AKKMap::GetAllCharactersOnMap(AKKCharacter* CharacterToExclude)
{
	TArray<AKKCharacter*> AllCharacters;
	for(int i = 0; i < TotalMapSize; i++)
	{
		if(const FMapCell* MapCell = GetCellAtIndex(i))
		{
			if(MapCell->Character != nullptr && MapCell->Character != CharacterToExclude)
			{
				AllCharacters.Add(MapCell->Character);
			}
		}
	}

	return AllCharacters;
}

TArray<AKKCharacter*> AKKMap::GetAllyCharactersOnMap(AKKCharacter* Character)
{
	return GetAllCharactersOnMap(Character).FilterByPredicate([=](AKKCharacter* AnotherCharacter) -> bool
	{
		return Character->IsInTheSameTeam(AnotherCharacter);
	});
}

TArray<AKKCharacter*> AKKMap::GetEnemyCharactersOnMap(AKKCharacter* Character)
{
	return GetAllCharactersOnMap(Character).FilterByPredicate([=](AKKCharacter* AnotherCharacter) -> bool
	{
		return !Character->IsInTheSameTeam(AnotherCharacter);
	});
}

bool AKKMap::AreCharactersInLine(AKKCharacter* Character, AKKCharacter* TargetCharacter) const
{
	const int32 TileID = Character->GetTilePositionID();
	const int32 TargetTileID = TargetCharacter->GetTilePositionID();

	if(TileID == -1 || TargetTileID == -1)
	{
		return false;
	}

	const FIntPoint Position = GetPositionByTileID(TileID);
	const FIntPoint TargetPosition = GetPositionByTileID(TargetTileID);

	return (Position.X == TargetPosition.X || Position.Y == TargetPosition.Y);
}

void AKKMap::GetTilesForBaseAttack(AKKCharacter* Character, TArray<AKKTile*>& InitialAttackTiles)
{
	const uint8 BaseIndex = Character->Direction == 1 ? 1 : 0; // if facing forward, character is attacking client, so client base index is 1
	
	if(CanAttackBase(Character))
	{
		InitialAttackTiles.Add(BaseArray[BaseIndex].Tile);
	}
}

bool AKKMap::CanAttackBase(const AKKCharacter* Character) const
{
	const uint8* BaseAttackTiles = (Character->Direction == 1) ? SecondBaseAttackTiles : FirstBaseAttackTiles;
	const uint8 TileID = Character->GetTilePositionID();

	if(TileID == BaseAttackTiles[0] || TileID == BaseAttackTiles[1])
	{
		return true;
	}

	return false;
}

void AKKMap::ClearTilesHighlights()
{
	for(int i = 0; i < TotalMapSize; i++)
	{
		GetCellAtIndex(i)->Tile->SetTileStatus(nullptr);
	}

	for(auto& BaseTile: BaseArray)
	{
		BaseTile.Tile->SetTileStatus(nullptr);
	}
}

AKKTile* AKKMap::GetTileAtIndex(uint8 TileID)
{
	if(TileID == UINT8_MAX || TileID == UINT8_MAX - 1)
	{
		return BaseArray[UINT8_MAX - TileID].Tile;
	}

	if(const FMapCell* MapCell = GetCellAtIndex(TileID))
	{
		return MapCell->Tile;
	}
	
	return nullptr;
}

AKKCharacter* AKKMap::GetCharacterAtIndex(uint8 TileID)
{
	if(TileID == UINT8_MAX || TileID == UINT8_MAX - 1)
	{
		return BaseArray[UINT8_MAX - TileID].Character;
	}
	
	return GetCellAtIndex(TileID) ? GetCellAtIndex(TileID)->Character : nullptr;
}

AKKCharacter* AKKMap::GetCharacterByPosition(const FIntPoint& Position)
{
	if(const FMapCell* MapCell = GetCellByPosition(Position))
	{
		return MapCell->Character;
	}

	return nullptr;
}

void AKKMap::RemoveCharacterFromTile(uint8 TileID)
{
	if(FMapCell* MapCell = GetCellAtIndex(TileID))
	{
		MapCell->Character = nullptr;
	}
}

void AKKMap::SpawnFraction(uint8 ID, TSubclassOf<AFraction> FractionClass)
{
	if(AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
	{
		TInlineComponentArray<UFractionComponent*, 2> FractionComponents;
		GetComponents(FractionComponents);

		if(FractionComponents.IsValidIndex(ID))
		{
			UFractionComponent* FractionComponent = FractionComponents[ID];
			if(FractionClass)
			{
				FractionComponent->FractionClass = FractionClass;
			}
		
			AFraction* FractionActor = GetWorld()->SpawnActor<AFraction>(FractionComponent->FractionClass, FractionComponent->GetComponentTransform());
			FractionActor->ID = FractionComponent->ID;

			TArray<AKKCharacter*> SpawnedCharacters = FractionActor->SpawnCharacters();
			AKKCharacter* FractionBase = FractionActor->SpawnBase();
		
			AssignCharacterToTile(FractionBase, &BaseArray[FractionComponent->ID]);
			SpawnedCharacters.Add(FractionBase);

			for(AKKCharacter* Character : SpawnedCharacters)
			{
				Character->SetOwner(GameMode->GetPlayerController(FractionComponent->ID));
			}
		}
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
	
	SetupMapTiles();
	SetupBaseTiles();
}

void AKKMap::SetupMapTiles()
{
	const uint8 SpacingX = 125;
	const uint8 SpacingY = 100;
	
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
}

void AKKMap::SetupBaseTiles()
{
	const float BaseLocationX[2] = { -338.f, 338.f};
	const float BaseRotationYaw[2] = { 90.f, 270.f};
	
	for (int32 i = 0; i < 2; i++)
	{
		AKKTile* Tile = GetWorld()->SpawnActor<AKKTile>(TileClass, FVector(BaseLocationX[i], 0.f, 0.f), FRotator(0.f, BaseRotationYaw[i], 0.f));
		Tile->TileID = UINT8_MAX - i;
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

FMapCell* AKKMap::GetCellAtIndex(uint8 TileID)
{
	const int32 X = GetX(TileID);
	const int32 Y = GetY(TileID);
	
	if(IsValidIndex(X, Y))
	{
		return &MapArray[X].MapRows[Y];
	}
	
	return nullptr;
}

FMapCell* AKKMap::GetCellByPosition(const FIntPoint& Position)
{
	if(IsValidIndex(Position.X, Position.Y))
	{
		return &MapArray[Position.X].MapRows[Position.Y];
	}

	return nullptr;
}

FMapCell* AKKMap::GetCellByDirection(const AKKCharacter* Character, const FDirection& Direction)
{
	if(Character->GetTilePositionID() != -1)
	{
		const int32 X = GetX(Character->OwnedTileID);
        const int32 Y = GetY(Character->OwnedTileID);
    
        const int32 CharacterDirection = Character->Direction;
        
        const int32 NextX = X + (CharacterDirection * Direction.X);
        const int32 NextY = Y + (CharacterDirection * Direction.Y);
        	
        if(IsValidIndex(NextX, NextY))
        {
        	return &MapArray[NextX].MapRows[NextY];
        }
	}
	
	return nullptr;
}

FIntPoint AKKMap::GetPositionByTileID(int32 TileID) const
{
	return {GetX(TileID), GetY(TileID)};
}

void AKKMap::SetFractionBase(uint8 ID, AKKCharacter* Base)
{
	AssignCharacterToTile(Base, &BaseArray[ID]);
}

void AKKMap::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AKKMap, MapArray);
	DOREPLIFETIME(AKKMap, BaseArray);
}

