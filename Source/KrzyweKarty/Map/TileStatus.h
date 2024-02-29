// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "UObject/Object.h"
#include "TileStatus.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class KRZYWEKARTY_API UTileStatus : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor StatusColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableCollision = false; // can be selected? (no for ability status)
	
};