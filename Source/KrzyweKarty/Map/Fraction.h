// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Fraction.generated.h"


class UArrowComponent;
class AKKCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class USpawnIndicator : public UBoxComponent
{
	GENERATED_BODY()

public:

	USpawnIndicator();
};


UCLASS()
class KRZYWEKARTY_API AFraction : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFraction();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* ArrowComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AKKCharacter>> CharactersToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AKKCharacter> BaseClass;

	TArray<AKKCharacter*> SpawnCharacters();
	AKKCharacter* SpawnBase() const;
	
	int32 ID;
protected:
	
	
	virtual void OnConstruction(const FTransform& Transform) override;


	UFUNCTION()
	void SpawnComponents();
	
	void RemoveComponents();
	
	virtual void BeginPlay() override;
	
};


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UFractionComponent : public UArrowComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFraction> FractionClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID = 0;
};