// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CharacterHelpersSettings.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Character Helpers Settings"))
class KRZYWEKARTY_API UCharacterHelpersSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Character Visuals")
	TSoftObjectPtr<UStaticMesh> PlatformMesh;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category="Character Visuals")
	TSoftObjectPtr<UMaterial> TextRenderMaterial;

	static const UCharacterHelpersSettings* Get()
	{
		if(Instance == nullptr)
		{
			Instance = GetDefault<UCharacterHelpersSettings>();
		}
		return Instance;
	}
private:

	static const inline UCharacterHelpersSettings* Instance = nullptr;
};
