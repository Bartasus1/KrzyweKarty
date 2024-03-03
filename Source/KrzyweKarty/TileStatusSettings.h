// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TileStatus/TileStatusDataAsset.h"

#include "TileStatusSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Tile Status Settings"))
class KRZYWEKARTY_API UTileStatusSettings : public UDeveloperSettings
{
	GENERATED_BODY() 
public:

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTileStatusDataAsset> TileStatusDataAsset;

	static const UTileStatusSettings* Get()
	{
		return GetDefault<UTileStatusSettings>();
	}

	static const UTileStatusDataAsset* GetDataAsset()
	{
		return GetDefault<UTileStatusSettings>()->TileStatusDataAsset.LoadSynchronous();
	}
};
