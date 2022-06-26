// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KKTile.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MapMesh;

	UPROPERTY(Replicated)
	TArray<AKKTile*> Tiles;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
