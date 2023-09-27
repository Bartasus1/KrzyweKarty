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
	//todo
}


void AKKGameMode::BeginPlay()
{
	Super::BeginPlay();

	while (Map == nullptr)
	{
		Map = Cast<AKKMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AKKMap::StaticClass()));
		//todo: Use Delegate to determine if Map has been Spawned
	}

	GetGameState<AKKGameState>()->Map = Map;
}

void AKKGameMode::ChangeTurn()
{
	Players[0]->bIsMyTurn = bFirstPlayerTurn;
	Players[1]->bIsMyTurn = !bFirstPlayerTurn;

	Players[0]->OnRep_TurnChanged();

	bFirstPlayerTurn = !bFirstPlayerTurn;
	
	GetGameState<AKKGameState>()->bFirstPlayerTurn = bFirstPlayerTurn;
	GetGameState<AKKGameState>()->OnRep_TurnChanged();
	
}

void AKKGameMode::SpawnCharacterForPlayer()
{
	if(Map)
	{
		Map->SpawnFraction(Players.Num() - 1, nullptr); //todo: find a way to allow player to choose a fraction in previous level and pass it here
	}
}

void AKKGameMode::AddActionLog(AKKCharacter* Character, AKKCharacter* TargetCharacter, FText Action)
{
	if(AKKPlayerController* PlayerController = Character->OwningPlayer)
	{
		FString PlayerName = PlayerController->PlayerState->GetPlayerName();

		FText Log = FText::FormatOrdered(FTextFormat::FromString("{0}: {1}({2}) {3} {4}") , FText::FromString(PlayerName), Character->GetCharacterName(), Character->CharacterID , Action, (TargetCharacter ? TargetCharacter->GetCharacterName() : FText::FromString(" ")));

		GetGameState<AKKGameState>()->AddActionLog(Log);
	}
}
