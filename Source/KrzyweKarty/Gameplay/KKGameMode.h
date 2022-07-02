// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KKCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "KKGameMode.generated.h"

class AKKMap;
class AKKCharacter;
class AKKPlayerController;

UCLASS()
class KRZYWEKARTY_API AKKGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKKGameMode();
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

public:
	void AddCharacterToMap(AKKCharacter* Character, int32 TileID, int32 PlayerID);

	void MoveForward(AKKCharacter* Character, int32 PlayerID);
	void MoveBackward(AKKCharacter* Character, int32 PlayerID);
	void MoveRight(AKKCharacter* Character, int32 PlayerID);
	void MoveLeft(AKKCharacter* Character, int32 PlayerID);

	void EndGameWithWinner(int32 PlayerID);

protected:
	virtual void BeginPlay() override;
	void IncreaseCounter();
	void ChangeTurn();

public:
	
	static int32 GetDistance(AKKCharacter* FirstCharacter, AKKCharacter* SecondCharacter);

protected:
	
	int8 MoveCounter = 0;
	bool FirstPlayerTurn = true;

	UPROPERTY(Transient)
	AKKMap* Map;

	UPROPERTY(Transient)
	TArray<AKKPlayerController*> Players;
};
