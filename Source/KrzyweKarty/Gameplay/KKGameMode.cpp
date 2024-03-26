// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameMode.h"

#include "KKGameState.h"
#include "KKPlayer.h"
#include "KKPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/UI/PlayerHUD.h"
#include "KrzyweKarty/Map/KKMap.h"
#include "Kismet/GameplayStatics.h"


AKKGameMode::AKKGameMode()
{
	GameStateClass = AKKGameState::StaticClass();
	PlayerControllerClass = AKKPlayerController::StaticClass();
	DefaultPawnClass = AKKPlayer::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
}

APlayerController* AKKGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	
	const FString PlayerStartTag = FString::FromInt(Players.Num() + 1); // 1 and 2 instead of 0 and 1 -> happens before creating player controller
	
	APlayerController* PlayerController =  Super::Login(NewPlayer, InRemoteRole, PlayerStartTag, Options, UniqueId, ErrorMessage);
	
	if (AKKPlayerController* KKPlayerController = Cast<AKKPlayerController>(PlayerController))
	{
		Players.Add(KKPlayerController);
		KKPlayerController->PlayerID = Players.Num();
		
		GetGameState<AKKGameState>()->RegisterPlayerInSystem(KKPlayerController);
		GetWorldTimerManager().SetTimerForNextTick(this, &AKKGameMode::SpawnCharacterForPlayer);
	}

	return PlayerController;
}

void AKKGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (Players.Num() == 2)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &AKKGameMode::ChangeTurn);
	}
}

void AKKGameMode::EndGameWithWinner(int32 PlayerID)
{
	AKKPlayerController* Winner = Players[PlayerID - 1];

	GetGameState<AKKGameState>()->DisplayWinner(Winner->PlayerState.Get());

	for(AKKPlayerController* PlayerController : Players)
	{
		PlayerController->bIsMyTurn = false; //prevent players from interacting with map
	}
}


void AKKGameMode::BeginPlay()
{
	Super::BeginPlay();

	while (Map == nullptr)
	{
		Map = Cast<AKKMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AKKMap::StaticClass()));
		//todo: Use Delegate to determine if Map has been Spawned or spawn it here
	}

	GetGameState<AKKGameState>()->Map = Map;
}

void AKKGameMode::ChangeTurn()
{
	GetGameState<AKKGameState>()->ChangeTurn();
}

void AKKGameMode::SpawnCharacterForPlayer()
{
	if(Map)
	{
		const int32 PlayerID = Players.Num() - 1;
		Map->SpawnFraction(PlayerID, PlayersFractions[PlayerID]);
	}
}
