// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "KKGameState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionLogAdded);

UCLASS()
class KRZYWEKARTY_API AKKGameState : public AGameStateBase
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	TArray<FText>& GetActionLogs();

	UFUNCTION()
	void OnRep_ActionLogs();

	UPROPERTY(BlueprintAssignable)
	FOnActionLogAdded ActionAdded;

	
	void AddActionLog(FText NewAction);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(ReplicatedUsing="OnRep_ActionLogs", BlueprintReadWrite)
	TArray<FText> ActionLogs;
	
};
