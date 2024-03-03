// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TileStatusDataAsset.generated.h"

class UTileStatus;
/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API UTileStatusDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTileStatus* AttackTileStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTileStatus* MovementTileStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTileStatus* SummonTileStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTileStatus* InActiveTileStatus;
};
