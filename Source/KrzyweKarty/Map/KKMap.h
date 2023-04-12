// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "KKMap.generated.h"

enum EMovementDirection : int;
class UStaticMeshComponent;
class AKKCharacter;
class AKKTile;

USTRUCT(BlueprintType)
struct FMapCell
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AKKTile* Tile;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AKKCharacter* Character;
};

USTRUCT(BlueprintType)
struct FMapRow
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FMapCell> MapRows;
};

UCLASS()
class KRZYWEKARTY_API AKKMap : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKKMap();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	TArray<FMapRow> MapArray;

	bool AddCharacterToMap(AKKCharacter* Character, int32 TileID);
	bool MoveCharacter(AKKCharacter* Character, EMovementDirection MovementDirection);
	
	// bool MoveForward(AKKCharacter* Character);
	// bool MoveBackward(AKKCharacter* Character);
	// bool MoveRight(AKKCharacter* Character);
	// bool MoveLeft(AKKCharacter* Character);
	
	TArray<AKKCharacter*> GetCharactersAtTiles(AKKCharacter* Character, TArray<TPair<int32, int32>> Tiles);

protected:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* MapMesh;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AKKTile> TileClass;

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void SetupMap();
	bool IsIndexValid(int32 X, int32 Y);

	////   Map values   ////
	const FVector StartLocation = FVector(-250.f, -150.f, 0.1);
	const uint8 MapSize = 4;

public:
	FORCEINLINE uint8 GetMapSize() const { return MapSize; }

private:
	FORCEINLINE int32 GetX(int32 TileID) const { return TileID / MapSize; }
	FORCEINLINE int32 GetY(int32 TileID) const { return TileID % MapSize; }
};
