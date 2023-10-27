// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundManagerSubsystem.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKGameState.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"

URoundManagerSubsystem::URoundManagerSubsystem()
{
}

void URoundManagerSubsystem::RegisterCharacterInSystem(AKKCharacter* Character)
{
	RegisteredCharacters.Add(Character);
	CountedMoves++;

	if(CountedMoves >= MovesPerTurn)
	{
		CountedMoves = 0;
		ChangeTurn();
	}

	UE_LOG(LogTemp, Warning, TEXT("CHARACTER REGISTERED"));
}

void URoundManagerSubsystem::RegisterPlayerInSystem(AKKPlayerController* PlayerController)
{
	RegisteredPlayers.AddUnique(PlayerController);
}

void URoundManagerSubsystem::ChangeTurn()
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
	GetWorld()->GetGameState<AKKGameState>()->TurnChanged.Broadcast();
}

TArray<AKKCharacter*> URoundManagerSubsystem::GetCharactersForPlayer(int32 PlayerID)
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

bool URoundManagerSubsystem::PlayerHasAnyLegalMoves(AKKPlayerController* PlayerController)
{
	TArray<AKKCharacter*> PlayerCharacters = GetCharactersForPlayer(PlayerController->PlayerID);

	//todo:
	
	return true;
}
