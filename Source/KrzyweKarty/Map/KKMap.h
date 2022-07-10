// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "KKMap.generated.h"

class UStaticMeshComponent;
class AKKCharacter;


UCLASS()
class KRZYWEKARTY_API AKKMap : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKKMap();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector> Tiles;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	TArray<AKKCharacter*> Characters;

	bool AddCharacterToMap(AKKCharacter* Character, int32 TileID);

	bool MoveForward(AKKCharacter* Character);
	bool MoveBackward(AKKCharacter* Character);
	bool MoveRight(AKKCharacter* Character);
	bool MoveLeft(AKKCharacter* Character);

protected:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* MapMesh;

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void SetupMap();
	bool CanMoveCharacter(int32 TileID, int32 NextTileID);
	bool AssignCharacterToTile(AKKCharacter* Character, int32 TileID);

	////   Map values   ////
	const FVector StartLocation = FVector(-250.f, -150.f, 0.1);
	const uint8 MapSize = 4;

public:
	FORCEINLINE int8 GetMapSize() const { return MapSize; }
};
