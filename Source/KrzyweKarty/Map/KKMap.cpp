// Fill out your copyright notice in the Description page of Project Settings.

#include "KKMap.h"

#include "KrzyweKarty/Interfaces/BaseInterface.h"

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

bool AKKMap::AddCharacterToMap(AKKCharacter* Character, int32 TileID)
{
	if (Character->OwnedTileID < 0 && Characters[TileID] == nullptr)
	{
		return AssignCharacterToTile(Character, TileID);
	}

	return false;
}

bool AKKMap::MoveForward(AKKCharacter* Character)
{
	int32 TileID = Character->OwnedTileID;

	if (TileID < 16 && CanMoveCharacter(TileID, TileID + 4))
	{
		Characters[TileID] = nullptr;

		return AssignCharacterToTile(Character, TileID + 4);
	}

	return false;
}

bool AKKMap::MoveBackward(AKKCharacter* Character)
{
	int32 TileID = Character->OwnedTileID;

	if (TileID > 3 && CanMoveCharacter(TileID, TileID - 4))
	{
		Characters[TileID] = nullptr;

		return AssignCharacterToTile(Character, TileID - 4);
	}

	return false;
}

bool AKKMap::MoveRight(AKKCharacter* Character)
{
	int32 TileID = Character->OwnedTileID;

	if (TileID % MapSize != 3 && CanMoveCharacter(TileID, TileID + 1))
	{
		Characters[TileID] = nullptr;

		return AssignCharacterToTile(Character, TileID + 1);
	}

	return false;
}

bool AKKMap::MoveLeft(AKKCharacter* Character)
{
	int32 TileID = Character->OwnedTileID;

	if (TileID % MapSize != 0 && CanMoveCharacter(TileID, TileID - 1))
	{
		Characters[TileID] = nullptr;

		return AssignCharacterToTile(Character, TileID - 1);
	}

	return false;
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

bool AKKMap::CanMoveCharacter(int32 TileID, int32 NextTileID)
{
	if (TileID < 0)
		return false;

	return (Characters.IsValidIndex(NextTileID) && Characters[NextTileID] == nullptr);
}

bool AKKMap::AssignCharacterToTile(AKKCharacter* Character, int32 TileID)
{
	Characters[TileID] = Character;

	Character->OwnedTileID = TileID;
	Character->SetActorLocation(Tiles[TileID]);
	return true;
}

void AKKMap::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKMap, Tiles);
	DOREPLIFETIME(AKKMap, Characters);
}

