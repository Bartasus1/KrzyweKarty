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

	UFUNCTION(Blueprintable)
	void ConfirmAbility();

protected:
	virtual void BeginPlay() override;
	virtual void SetOwner(AActor* NewOwner) override;

public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AKKCharacter* Character;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	uint8 AbilityIndex = 0;

	UPROPERTY(VisibleAnywhere)
	UAbilityAction* AbilityAction;
};
