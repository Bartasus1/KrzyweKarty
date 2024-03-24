// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameState.h"

#include "Kismet/GameplayStatics.h"

#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"
#include "KrzyweKarty/UI/PlayerHUD.h"

#include "Net/UnrealNetwork.h"


void AKKGameState::RegisterCharacterInSystem_Implementation(AKKCharacter* Character)
{
	RegisteredCharacters.Add(Character); // Set -> no duplicates
	CountedMoves++;

	if(CountedMoves >= MovesPerTurn)
	{
		ChangeTurn();
	}
}

void AKKGameState::RegisterPlayerInSystem_Implementation(AKKPlayerController* PlayerController)
{
	RegisteredPlayers.AddUnique(PlayerController);
}

void AKKGameState::ChangeTurn_Implementation()
{
	bFirstPlayerTurn = !bFirstPlayerTurn;
	
	RegisteredPlayers[0]->bIsMyTurn = !bFirstPlayerTurn;
	RegisteredPlayers[1]->bIsMyTurn = bFirstPlayerTurn;
	
	RegisteredPlayers[0]->OnRep_TurnChanged();
	RegisteredPlayers[1]->OnRep_TurnChanged();

	for(AKKCharacter* Character : RegisteredCharacters)
	{
		if(Character != nullptr)
		{
			Character->CharacterActions = 0;
		}
	}
	
	CountedMoves = 0;
}

void AKKGameState::DisplayWinner_Implementation(APlayerState* Winner)
{
	if(AKKPlayerController* PlayerController = Cast<AKKPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if(PlayerController->GetHUD())
        {
        	PlayerController->GetHUD<APlayerHUD>()->DisplayWinner(Winner);
        }
	}

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


void AKKGameState::Server_AddActionLog_Implementation(const FText& NewAction)
{
	Multicast_AddActionLog(NewAction, true);
}

void AKKGameState::Multicast_AddActionLog_Implementation(const FText& NewAction, bool bIsFromServer)
{
	ActionAdded.Broadcast(NewAction, bIsFromServer);
}

void AKKGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKGameState, Map);
	DOREPLIFETIME(AKKGameState, bFirstPlayerTurn);
}
