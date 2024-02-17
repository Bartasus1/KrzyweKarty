// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "KKGameState.generated.h"

class AKKCharacter;
class AKKPlayerController;
class AKKMap;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionLogAdded, const FText&, InText, bool, bIsFromServer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnChanged);

UCLASS()
class KRZYWEKARTY_API AKKGameState : public AGameStateBase
{
	GENERATED_BODY()
public:

	UPROPERTY(Replicated, BlueprintReadOnly)
	AKKMap* Map;
	
	UPROPERTY(BlueprintAssignable)
	FOnActionLogAdded ActionAdded;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AddActionLog(const FText& NewAction);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_AddActionLog(const FText& NewAction, bool bIsFromServer = false);

	//////////////////////////////////////////////////////
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bFirstPlayerTurn = true;

	UFUNCTION(Server, Reliable)
	void RegisterCharacterInSystem(AKKCharacter* Character);
	
	UFUNCTION(Server, Reliable)
	void RegisterPlayerInSystem(AKKPlayerController* PlayerController);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangeTurn();

private:
	
	TArray<AKKCharacter*> GetCharactersForPlayer(int32 PlayerID);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<AKKPlayerController*> RegisteredPlayers;

	UPROPERTY(VisibleAnywhere)
	TSet<AKKCharacter*> RegisteredCharacters;

	const int32 MovesPerTurn = 3;
	int32 CountedMoves = 0;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
