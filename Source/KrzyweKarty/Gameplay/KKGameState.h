// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "KKGameState.generated.h"

class AKKMap;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionLogAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnChanged);

UCLASS()
class KRZYWEKARTY_API AKKGameState : public AGameStateBase
{
	GENERATED_BODY()
public:

	
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bFirstPlayerTurn = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	AKKMap* Map;


	UPROPERTY(BlueprintAssignable)
	FOnActionLogAdded ActionAdded;

	UPROPERTY(BlueprintAssignable)
	FOnTurnChanged TurnChanged;
	
	UFUNCTION(BlueprintCallable)
	TArray<FText>& GetActionLogs();
	
	UFUNCTION()
	void OnRep_ActionLogs();

	UFUNCTION(Server, Reliable)
	void AddActionLog(const FText& NewAction);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(ReplicatedUsing="OnRep_ActionLogs", Transient)
	TArray<FText> ActionLogs;
	
};
