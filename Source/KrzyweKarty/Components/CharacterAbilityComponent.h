// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterAbilityComponent.generated.h"

class AKKPlayerController;
class AKKGameState;
class AKKMap;
class AKKCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterAbilityEvent);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class KRZYWEKARTY_API UCharacterAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterAbilityComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginAbility();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinishAbility(bool bWasCancelled);

	UPROPERTY(BlueprintAssignable)
	FCharacterAbilityEvent OnAbilityStarted;

	UPROPERTY(BlueprintAssignable)
	FCharacterAbilityEvent OnAbilityCanceled;

	UPROPERTY(BlueprintAssignable)
	FCharacterAbilityEvent OnAbilityFinished;

	UPROPERTY(EditAnywhere)
	int32 AbilityIndex = 1;

protected:
	
	UPROPERTY(BlueprintReadOnly)
	AKKCharacter* OwnerCharacter = nullptr;

	UFUNCTION(BlueprintPure)
	AKKMap* GetMap();

	UFUNCTION(BlueprintPure)
	AKKGameState* GetGameState();

	UFUNCTION(BlueprintPure)
	AKKPlayerController* GetPlayerController();

public:
	virtual void BeginPlay() override;
};
