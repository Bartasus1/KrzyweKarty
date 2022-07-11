// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KKGameMode.generated.h"

class AKKMap;
class AKKCharacter;
class AKKPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerJoinedDelegate);

UCLASS()
class KRZYWEKARTY_API AKKGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKKGameMode();

	UPROPERTY(BlueprintAssignable)
	FPlayerJoinedDelegate OnPlayerJoined;
	
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
	void IncreaseMovesCounter();

protected:
	virtual void BeginPlay() override;
	void ChangeTurn();

private:
	int8 MoveCounter = 0;
	bool FirstPlayerTurn = true;

protected:
	UPROPERTY(Transient)
	AKKMap* Map;

	UPROPERTY(Transient)
	TArray<AKKPlayerController*> Players;

public:
	FORCEINLINE AKKMap* GetMap() const { return Map; }
	FORCEINLINE AKKPlayerController* GetPlayerController(int32 PlayerID) const { return Players.IsValidIndex(PlayerID) ? Players[PlayerID] : nullptr; }
};
