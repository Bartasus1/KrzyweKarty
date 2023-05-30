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
	
	if (GetCellAtIndex(TileID) && GetCellAtIndex(TileID)->Character == nullptr && !Character->IsCharacterOnMap())
	{
		AssignCharacterToTile(Character, TileID);
		return true;
	}

	return false;
}

bool AKKMap::MoveCharacter(AKKCharacter* Character, EMovementDirection MovementDirection)
{
	const int32 NewTileID = Character->OwnedTileID + MovementDirection;
	
	if(GetCellAtIndex(NewTileID) && GetCellAtIndex(NewTileID)->Character == nullptr && Character->IsCharacterOnMap())
	{
		GetCellAtIndex(Character->OwnedTileID)->Character = nullptr;
		AssignCharacterToTile(Character, NewTileID);

		return true;
	}

	return false;
}

TArray<AKKCharacter*> AKKMap::GetCharactersAtTiles(AKKCharacter* Character, TArray<TPair<int32, int32>> RelativeTiles)
{
	TArray<AKKCharacter*> FoundCharacters;

	const int32 X = GetX(Character->OwnedTileID);
	const int32 Y = GetY(Character->OwnedTileID);

	const int32 Direction = (Character->OwningPlayer->PlayerID == 1) ? 1 : -1;
	
	for(const auto& Tile : RelativeTiles)
	{
		const int32 NextX = X + (Direction * Tile.Key);
		const int32 NextY = Y + (Direction * Tile.Value);
		
		if(MapArray.IsValidIndex(NextX))
		{
			
			
			if(MapArray[NextX].MapRows.IsValidIndex(NextY))
			{
				// int32 id = NextY + NextX * MapSize;
				// UE_LOG(LogTemp, Warning, TEXT("TILE_ID: %d"), id);
				
				FoundCharacters.Add(MapArray[NextX].MapRows[NextY].Character);
			}
		}
	}

	return FoundCharacters;
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

			MapArray[i].MapRows.Add({Tile, nullptr});
		}
	}
}

void AKKMap::AssignCharacterToTile(AKKCharacter* Character, int32 TileID)
{
	GetCellAtIndex(TileID)->Character = Character;
		
	Character->OwnedTileID = TileID;
	Character->SetActorLocation(GetCellAtIndex(TileID)->Tile->GetActorLocation());
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

