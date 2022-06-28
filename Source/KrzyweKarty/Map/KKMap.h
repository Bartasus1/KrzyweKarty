// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KrzyweKarty/Gameplay/KKCharacter.h"
#include "Net/UnrealNetwork.h"
#include "KKMap.generated.h"

class UStaticMeshComponent;


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

	void AddCharacterToMap(AKKCharacter* Character, int32 TileID);

	void MoveForward(AKKCharacter* Character);
	void MoveBackward(AKKCharacter* Character);
	void MoveRight(AKKCharacter* Character);
	void MoveLeft(AKKCharacter* Character);

protected:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* MapMesh;

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void SetupMap();
	void AssignCharacterToTile(AKKCharacter* Character, int32 TileID);

	////   Map values   ////
	const FVector StartLocation = FVector(-250.f, -150.f, 0.1);
	const uint8 MapSize = 4;
	
};
