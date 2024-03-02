// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "TileStatus.generated.h"

class UAction;
/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API UTileStatus : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FLinearColor StatusColor = FLinearColor(0,0,0,1);

	UPROPERTY(EditAnywhere, meta=(ToolTip="Is this status used later in deciding character action?"))
	bool bEnableCollision = false; // can be selected? (no for ability status)

	UPROPERTY(EditAnywhere)
	FGameplayTag TileStatusTag;
};