// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityActor.generated.h"

class UAbilityAction;
class AKKCharacter;

UCLASS()
class KRZYWEKARTY_API AAbilityActor : public AActor
{
	GENERATED_BODY()

public:
	AAbilityActor();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void BeginAbility();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnBeginAbility();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnFinishAbility();
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ConfirmAbility();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void AbortAbility();

protected:
	virtual void BeginPlay() override;
public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(ExposeOnSpawn))
	AKKCharacter* Character;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(ExposeOnSpawn))
	uint8 AbilityIndex = 0;

	UPROPERTY(VisibleAnywhere)
	UAbilityAction* AbilityAction;
};
