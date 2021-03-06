// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AreaModifierInterface.generated.h"

class AKKCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAreaModifierInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KRZYWEKARTY_API IAreaModifierInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	virtual TArray<AKKCharacter*> GetAffectedCharacters() = 0;
};
