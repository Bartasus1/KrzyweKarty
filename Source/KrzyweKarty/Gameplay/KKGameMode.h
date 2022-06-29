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

	virtual void PostLogin(APlayerController* NewPlayer) override;

	bool AddCharacterToMap(AKKCharacter* Character, int32 TileID, int32 PlayerID);

	bool MoveForward(AKKCharacter* Character, int32 PlayerID);
	bool MoveBackward(AKKCharacter* Character, int32 PlayerID);
	bool MoveRight(AKKCharacter* Character, int32 PlayerID);
	bool MoveLeft(AKKCharacter* Character, int32 PlayerID);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	AKKMap* Map;
};
