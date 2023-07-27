// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilityExecutor.generated.h"

enum ERotationDirection : int;
class AKKGameState;
class AKKMap;
class AKKCharacter;
UCLASS()
class KRZYWEKARTY_API AAbilityExecutor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAbilityExecutor();

protected:
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	int32 Index;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	AKKCharacter* Character;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SR_ExecuteAbility();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SR_EndAbility();
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BeginAbility();

	UFUNCTION(BlueprintImplementableEvent)
	void ExecuteAbility();

	UFUNCTION(BlueprintImplementableEvent)
	void EndAbility();

protected:
	//Helper functions

	UFUNCTION(BlueprintPure)
	AKKMap* GetMap();

	UFUNCTION(BlueprintPure)
	AKKGameState* GetGameState();

	UFUNCTION(BlueprintPure)
	ERotationDirection GetRotationFromInt(int32 Number);
};
