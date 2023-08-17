// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilityHelperComponent.generated.h"

class AKKPlayerController;
class AKKGameState;
class AKKMap;
class AKKCharacter;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class KRZYWEKARTY_API UAbilityHelperComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityHelperComponent();

protected:
	
	UPROPERTY(BlueprintReadWrite)
	AKKCharacter* OwnerCharacter;

	UFUNCTION(BlueprintPure)
	AKKMap* GetMap();

	UFUNCTION(BlueprintPure)
	AKKGameState* GetGameState();

	UFUNCTION(BlueprintPure)
	AKKPlayerController* GetPlayerController();

public:
	virtual void BeginPlay() override;
};
