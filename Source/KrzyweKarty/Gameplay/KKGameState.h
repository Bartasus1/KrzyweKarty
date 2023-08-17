// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "KKGameState.generated.h"

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

	UPROPERTY(ReplicatedUsing="OnRep_ActionLogs", Transient)
	TArray<FLogStruct> ActionLogs;
	
	UPROPERTY(ReplicatedUsing="OnRep_TurnChanged", BlueprintReadOnly)
	bool bFirstPlayerTurn = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	AKKMap* Map;
	
	UPROPERTY(BlueprintAssignable)
	FOnActionLogAdded ActionAdded;

	UPROPERTY(BlueprintAssignable)
	FOnTurnChanged TurnChanged;

public:
	
	UFUNCTION()
	void OnRep_ActionLogs();

	UFUNCTION()
	void OnRep_TurnChanged();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void AddActionLog(const FText& NewAction);

	UFUNCTION(BlueprintCallable)
	TArray<FLogStruct>& GetActionLogs();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
