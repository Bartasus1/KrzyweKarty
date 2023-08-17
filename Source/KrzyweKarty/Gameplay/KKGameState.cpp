// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameState.h"
#include "Net/UnrealNetwork.h"


TArray<FLogStruct>& AKKGameState::GetActionLogs()
{
	return ActionLogs;
}

void AKKGameState::OnRep_ActionLogs()
{
	ActionAdded.Broadcast();
}

void AKKGameState::OnRep_TurnChanged()
{
	TurnChanged.Broadcast();
}

void AKKGameState::AddActionLog_Implementation(const FText& NewAction)
{
	ActionLogs.Add({NewAction, true});
	OnRep_ActionLogs();
}

void AKKGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKGameState, Map);
	DOREPLIFETIME(AKKGameState, ActionLogs);
	DOREPLIFETIME(AKKGameState, bFirstPlayerTurn);
}
