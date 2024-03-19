// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AreaAbilityInterface.generated.h"

class AKKCharacter;
// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable)
class UAreaAbilityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KRZYWEKARTY_API IAreaAbilityInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<AKKCharacter*> GetAffectedCharacters();
};
