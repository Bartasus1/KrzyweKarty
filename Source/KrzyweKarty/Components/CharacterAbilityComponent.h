// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAbilityComponent.generated.h"

class AKKPlayerController;
class AKKGameState;
class AKKMap;
class AKKCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType, Abstract)
class KRZYWEKARTY_API UCharacterAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterAbilityComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnBeginAbility(int32 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnFinishAbility(int32 Index);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 AbilityIndex = 0;

protected:
	
	UPROPERTY(BlueprintReadOnly)
	AKKCharacter* OwnerCharacter = nullptr;

	UFUNCTION(BlueprintPure)
	AKKMap* GetMap() const;

	UFUNCTION(BlueprintPure)
	AKKGameState* GetGameState() const;

	UFUNCTION(BlueprintPure)
	AKKPlayerController* GetPlayerController() const;

public:
	virtual void BeginPlay() override;
};
