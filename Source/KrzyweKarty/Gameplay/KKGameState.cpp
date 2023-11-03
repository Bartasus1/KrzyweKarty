// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameState.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"
#include "Net/UnrealNetwork.h"

TArray<FLogStruct>& AKKGameState::GetActionLogs()
{
	return ActionLogs;
}

void AKKGameState::RegisterCharacterInSystem_Implementation(AKKCharacter* Character)
{
	RegisteredCharacters.Add(Character);
	CountedMoves++;

	if(CountedMoves >= MovesPerTurn)
	{
		CountedMoves = 0;
		ChangeTurn();
	}
}

void AKKGameState::RegisterPlayerInSystem_Implementation(AKKPlayerController* PlayerController)
{
	RegisteredPlayers.AddUnique(PlayerController);
}

void AKKGameState::ChangeTurn_Implementation()
{
	RegisteredPlayers[0]->bIsMyTurn = bFirstPlayerTurn;
	RegisteredPlayers[1]->bIsMyTurn = !bFirstPlayerTurn;

	RegisteredPlayers[0]->OnRep_TurnChanged();

	for(AKKCharacter* Character : RegisteredCharacters)
	{
		if(Character != nullptr)
		{
			Character->CharacterActions.Reset();
		}
	}

	bFirstPlayerTurn = !bFirstPlayerTurn;
	CountedMoves = 0;
}

TArray<AKKCharacter*> AKKGameState::GetCharactersForPlayer(int32 PlayerID)
{
	TArray<AKKCharacter*> PlayerCharacters;

	for(AKKCharacter* Character : RegisteredCharacters)
	{
		if(Character->OwningPlayer == RegisteredPlayers[PlayerID])
		{
			PlayerCharacters.Add(Character);
		}
	}

	return PlayerCharacters;
}

void AKKGameState::OnRep_ActionLogs()
{
	ActionAdded.Broadcast();
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
