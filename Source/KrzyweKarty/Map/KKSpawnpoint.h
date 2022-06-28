// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "KKSpawnpoint.generated.h"

class AKKCharacter;

USTRUCT(BlueprintType)
struct FSpawnSet : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AKKCharacter> ClassToSpawn;
};

UCLASS()
class KRZYWEKARTY_API AKKSpawnpoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKKSpawnpoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MustImplement="FSpawnSet"))
	UDataTable* SpawnTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<AKKCharacter*> SpawnedCards;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnCards();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
