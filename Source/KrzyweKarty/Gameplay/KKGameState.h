// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "KKGameState.generated.h"

class AKKCharacter;
class AKKPlayerController;
class AKKMap;

USTRUCT(BlueprintType)
struct FLogStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FText LogText;

	UPROPERTY(BlueprintReadOnly)
	bool bIsSentByServer = true;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionLogAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnChanged);

UCLASS()
class KRZYWEKARTY_API AKKGameState : public AGameStateBase
{
	GENERATED_BODY()
public:

	UPROPERTY(Replicated, BlueprintReadOnly)
	AKKMap* Map;

	///////////////////////////////////////////
	UPROPERTY(ReplicatedUsing="OnRep_ActionLogs", Transient)
	TArray<FLogStruct> ActionLogs;
	
	UPROPERTY(BlueprintAssignable)
	FOnActionLogAdded ActionAdded;
	
	UFUNCTION()
	void OnRep_ActionLogs();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void AddActionLog(const FText& NewAction);

	UFUNCTION(BlueprintCallable)
	TArray<FLogStruct>& GetActionLogs();

	//////////////////////////////////////////////////////
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bFirstPlayerTurn = true;

	UFUNCTION(Server, Reliable)
	void RegisterCharacterInSystem(AKKCharacter* Character);
	
	UFUNCTION(Server, Reliable)
	void RegisterPlayerInSystem(AKKPlayerController* PlayerController);

	UFUNCTION(Server, Reliable)
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
