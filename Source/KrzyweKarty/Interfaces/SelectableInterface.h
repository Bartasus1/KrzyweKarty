// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SelectableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, NotBlueprintable)
class USelectableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KRZYWEKARTY_API ISelectableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable)
	virtual int32 GetTilePositionID() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual bool IsSelectable() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual void OnSelectableHighlighted() = 0;
};
