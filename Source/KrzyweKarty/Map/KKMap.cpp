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
	int32 X = GetX(TileID);
	int32 Y = GetY(TileID);
	
	if (Character->OwnedTileID < 0 && MapArray[X].MapRows[Y].Character == nullptr && !IsCharacterOnMap(Character))
	{
		MapArray[X].MapRows[Y].Character = Character;
		Characters.Add(Character);
		
		Character->OwnedTileID = TileID;
		Character->SetActorLocation(MapArray[X].MapRows[Y].Tile->GetActorLocation());
		return true;
	}

	return false;
}

bool AKKMap::MoveCharacter(AKKCharacter* Character, EMovementDirection MovementDirection)
{
	const int32 X = GetX(Character->OwnedTileID);
	const int32 Y = GetY(Character->OwnedTileID);

	const int32 NewTileID = Character->OwnedTileID + MovementDirection;
	const int32 NewX = NewTileID / MapSize;
	const int32 NewY = NewTileID % MapSize;

	if(IsIndexValid(NewX, NewY) && MapArray[NewX].MapRows[NewY].Character == nullptr && IsCharacterOnMap(Character))
	{
		MapArray[X].MapRows[Y].Character = nullptr;
		MapArray[NewX].MapRows[NewY].Character = Character;
		
		Character->OwnedTileID = NewTileID;
		Character->SetActorLocation(MapArray[NewX].MapRows[NewY].Tile->GetActorLocation());
		return true;
	}

	return false;
}

TArray<AKKCharacter*> AKKMap::GetCharactersAtTiles(AKKCharacter* Character, TArray<TPair<int32, int32>> RelativeTiles)
{
	TArray<AKKCharacter*> Characters;

	const int32 X = GetX(Character->OwnedTileID);
	const int32 Y = GetY(Character->OwnedTileID);

	const int32 Direction = (Character->OwningPlayer->PlayerID == 1) ? 1 : -1;
	
	for(const auto& Tile : RelativeTiles)
	{
		const int32 NextX = X + (Direction * Tile.Key);
		
		if(MapArray.IsValidIndex(NextX))
		{
			const int32 NextY = Y + (Direction * Tile.Value);
			
			if(MapArray[NextX].MapRows.IsValidIndex(NextY))
			{
				// int32 id = NextY + NextX * MapSize;
				// UE_LOG(LogTemp, Warning, TEXT("TILE_ID: %d"), id);
				
				Characters.Add(MapArray[NextX].MapRows[NextY].Character);
			}
		}
	}

	return Characters;
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
		MapArray.Add(FMapRow()); // 5 rows with 4 cols
		
		for (int j = 0; j < 4; j++)
		{
			FVector TileLocation = StartLocation;
			TileLocation.X += i * SpacingX;
			TileLocation.Y += j * SpacingY;
			
			AKKTile* Tile = GetWorld()->SpawnActor<AKKTile>(TileClass, TileLocation, GetActorRotation());
			Tile->TileID = j + i * MapSize;

			MapArray[i].MapRows.Add({Tile, nullptr});
		}
	}

}

bool AKKMap::IsIndexValid(int32 X, int32 Y)
{
	return MapArray.IsValidIndex(X) && MapArray[X].MapRows.IsValidIndex(Y);
}


void AKKMap::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AKKMap, MapArray);
	DOREPLIFETIME(AKKMap, Characters);
}

