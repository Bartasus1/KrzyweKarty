// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KrzyweKarty/Map/KKMap.h"
#include "KKGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AKKGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKKGameMode();

	bool AddCharacterToMap(AKKCharacter* Character, int32 TileID);

	bool MoveForward(AKKCharacter* Character, int32 ID);
	bool MoveBackward(AKKCharacter* Character, int32 ID);
	bool MoveRight(AKKCharacter* Character, int32 ID);
	bool MoveLeft(AKKCharacter* Character, int32 ID);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	AKKMap* Map;
};
