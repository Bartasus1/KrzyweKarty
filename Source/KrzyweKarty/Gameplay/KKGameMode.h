// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KKPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "KKGameMode.generated.h"

class AFraction;
class UActionLog;
class AKKMap;
class AKKCharacter;
class URoundManager;

UCLASS()
class KRZYWEKARTY_API AKKGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKKGameMode();

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
	UPROPERTY(Transient, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	AKKMap* Map = nullptr;

	UPROPERTY(Transient)
	TArray<AKKPlayerController*> Players;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<AFraction>> PlayersFractions;

public:
	FORCEINLINE AKKMap* GetMap() const { return Map; }
	FORCEINLINE AKKPlayerController* GetPlayerController(int32 PlayerID) const { return Players.IsValidIndex(PlayerID) ? Players[PlayerID] : nullptr; }
};
