// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameState.h"
#include "Net/UnrealNetwork.h"

TArray<FText>& AKKGameState::GetActionLogs()
{
	return ActionLogs;
}

void AKKGameState::OnRep_ActionLogs()
{
	ActionAdded.Broadcast();
}

void AKKGameState::AddActionLog_Implementation(const FText& NewAction)
{
	ActionLogs.Add(NewAction);
	OnRep_ActionLogs();
}

void AKKGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKGameState, ActionLogs);
}
