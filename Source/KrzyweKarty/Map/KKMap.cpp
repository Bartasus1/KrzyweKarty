// Fill out your copyright notice in the Description page of Project Settings.

#include "KKMap.h"

// Sets default values
AKKMap::AKKMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	MapMesh = CreateDefaultSubobject<UStaticMeshComponent>("MapMesh");

	MapMesh->SetRelativeScale3D(FVector(1.072922, 0.949018, 1));

	Tiles.Reserve(22); // tiles + 2 base cards
	Characters.Init(nullptr, 22);
}

void AKKMap::AddCharacterToMap(AKKCharacter *Character, int32 TileID)
{
	if (Character->OwnedTileID < 0)
	{
		AssignCharacterToTile(Character, TileID);
	}
}

void AKKMap::MoveForward(AKKCharacter *Character)
{
	int32 TileID = Character->OwnedTileID;

	if (TileID < 0)
		return;

	if (TileID < 16 && Characters.IsValidIndex(TileID + 4))
	{
		if (Characters[TileID + 4] != nullptr)
			return;

		Characters[TileID] = nullptr;

		AssignCharacterToTile(Character, TileID + 4);
	}
}

void AKKMap::MoveBackward(AKKCharacter *Character)
{
	int32 TileID = Character->OwnedTileID;

	if (TileID < 0)
		return;

	if (TileID > 3 && Characters.IsValidIndex(TileID - 4))
	{
		if (Characters[TileID - 4] != nullptr)
			return;

		Characters[TileID] = nullptr;

		AssignCharacterToTile(Character, TileID - 4);
	}
}

void AKKMap::MoveRight(AKKCharacter *Character)
{
	int32 TileID = Character->OwnedTileID;

	if (TileID < 0)
		return;

	if (TileID % MapSize != 3 && Characters.IsValidIndex(TileID + 1))
	{
		if (Characters[TileID + 1] != nullptr)
			return;

		Characters[TileID] = nullptr;

		AssignCharacterToTile(Character, TileID + 1);
	}
}

void AKKMap::MoveLeft(AKKCharacter *Character)
{
	int32 TileID = Character->OwnedTileID;

	if (TileID < 0)
		return;

	if (TileID % MapSize != 0 && Characters.IsValidIndex(TileID - 1))
	{
		if (Characters[TileID - 1] != nullptr)
			return;

		Characters[TileID] = nullptr;

		AssignCharacterToTile(Character, TileID - 1);
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
	constexpr uint16 SpacingX = 125;
	constexpr uint16 SpacingY = 100;

	for (int i = 0; i < 20; i++)
	{
		FVector TileLocation = StartLocation;
		TileLocation.X += (i / MapSize) * SpacingX;
		TileLocation.Y += (i % MapSize) * SpacingY;

		Tiles.Add(TileLocation);
	}
}

void AKKMap::AssignCharacterToTile(AKKCharacter *Character, int32 TileID)
{
	Characters[TileID] = Character;

	Character->OwnedTileID = TileID;
	Character->SetActorLocation(Tiles[TileID]);
}

void AKKMap::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKMap, Tiles);
	DOREPLIFETIME(AKKMap, Characters);
}
