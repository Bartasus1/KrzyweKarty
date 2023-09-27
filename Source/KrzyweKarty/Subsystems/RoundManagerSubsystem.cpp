// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundManagerSubsystem.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"

void URoundManagerSubsystem::RegisterCharacterInSystem(AKKCharacter* Character)
{
	RegisteredCharacters.Add(Character);
	CountedMoves++;

	if(CountedMoves >= MovesPerTurn)
	{
		CountedMoves = 0;
		ChangeTurn();
	}
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
		Character->CharacterActions.Reset();
	}

	bFirstPlayerTurn = !bFirstPlayerTurn;
}
