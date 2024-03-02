// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Map/TileStatus.h"
#include "TileStatusSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Tile Status Settings"))
class KRZYWEKARTY_API UTileStatusSettings : public UDeveloperSettings
{
	GENERATED_BODY() //todo: objects cannot be assigned :/
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTileStatus* AttackTileStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTileStatus* MovementTileStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTileStatus* SummonTileStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTileStatus* InActiveTileStatus;

	static const UTileStatusSettings* Get()
	{
		return GetDefault<UTileStatusSettings>();
	}
};
