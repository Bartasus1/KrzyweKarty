// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KKPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "KKGameMode.generated.h"

class UActionLog;
class AKKMap;
class AKKCharacter;
class URoundManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerJoinedDelegate);

UCLASS()
class KRZYWEKARTY_API AKKGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKKGameMode();

	UPROPERTY(BlueprintAssignable)
	FPlayerJoinedDelegate OnPlayerJoined; // assign cards to player on join

protected:
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:

	void EndGameWithWinner(int32 PlayerID);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ChangeTurn();

	UFUNCTION()
	void SpawnCharacterForPlayer();

private:
	bool bFirstPlayerTurn = true;

	void AddActionLog(AKKCharacter* Character, AKKCharacter* TargetCharacter, FText Action);

protected:
	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly)
	AKKMap* Map = nullptr;

	UPROPERTY(Transient)
	TArray<AKKPlayerController*> Players;

public:
	FORCEINLINE AKKMap* GetMap() const { return Map; }
	FORCEINLINE AKKPlayerController* GetPlayerController(int32 PlayerID) const { return Players.IsValidIndex(PlayerID) ? Players[PlayerID] : nullptr; }
};
