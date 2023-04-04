// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameMode.h"

#include "KKGameState.h"
#include "KKPlayer.h"
#include "KKPlayerController.h"
#include "RoundManager.h"
#include "GameFramework/PlayerState.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/UI/PlayerHUD.h"
#include "KrzyweKarty/Map/KKMap.h"
#include "Kismet/GameplayStatics.h"


AKKGameMode::AKKGameMode()
{
	RoundManager = CreateDefaultSubobject<URoundManager>("RoundManager");

	RoundManager->OnRoundEnd.AddUniqueDynamic(this, &AKKGameMode::ChangeTurn);

	GameStateClass = AKKGameState::StaticClass();
	PlayerControllerClass = AKKPlayerController::StaticClass();
	DefaultPawnClass = AKKPlayer::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
}

APlayerController* AKKGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	
	const FString PlayerStartTag = FString::FromInt(Players.Num() + 1); // 1 and 2 instead of 0 and 1
	
	APlayerController* PlayerController =  Super::Login(NewPlayer, InRemoteRole, PlayerStartTag, Options, UniqueId, ErrorMessage);
	
	if (AKKPlayerController* KKPlayerController = Cast<AKKPlayerController>(PlayerController))
	{
		Players.Add(KKPlayerController);

		KKPlayerController->PlayerID = Players.Num();

		OnPlayerJoined.Broadcast();
	}

	return PlayerController;
}

void AKKGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (Players.Num() == 2)
	{
		ChangeTurn();
	}
}

void AKKGameMode::AddCharacterToMap(AKKCharacter* Character, int32 TileID, int32 PlayerID)
{
	TileID = (PlayerID == 1) ? TileID : 19 - TileID;

	if (Map->AddCharacterToMap(Character, TileID))
	{
		RoundManager->AddCharacterToList(Character, EMP_SummonedCharacter);
		AddActionLog(Character, nullptr, FText::FromString(" dodany do mapy"));
	}
}

void AKKGameMode::MoveCharacter(AKKCharacter* Character, EMovementDirection MovementDirection, int32 PlayerID)
{
	if (RoundManager->CanUseCharacter(Character, EMP_MovedCharacter))
	{
		auto MoveFunction = &AKKMap::MoveForward;

		switch (MovementDirection)
		{
		case EMD_Forward:
			MoveFunction = (PlayerID == 1) ? &AKKMap::MoveForward : &AKKMap::MoveBackward;
			break;
		case EMD_Backward:
			MoveFunction = (PlayerID == 1) ? &AKKMap::MoveBackward : &AKKMap::MoveForward;
			break;
		case EMD_Right:
			MoveFunction = (PlayerID == 1) ? &AKKMap::MoveRight : &AKKMap::MoveLeft;
			break;
		case EMD_Left:
			MoveFunction = (PlayerID == 1) ? &AKKMap::MoveLeft : &AKKMap::MoveRight;
			break;
		default:
			break;
		}

		
		
		if ((Map->*MoveFunction)(Character))
		{
			RoundManager->AddCharacterToList(Character, EMP_MovedCharacter);
			AddActionLog(Character, nullptr, FText::FromString("ruszyl sie " + UEnum::GetDisplayValueAsText(MovementDirection).ToString()));
		}
	}
}

void AKKGameMode::PerformCharacterAttack(AKKCharacter* Character, AKKCharacter* TargetCharacter)
{
	if (RoundManager->CanUseCharacter(Character, EMP_AttackCharacter))
	{
		if (Character->DefaultAttack(TargetCharacter))
		{
			RoundManager->AddCharacterToList(Character, EMP_AttackCharacter);
			AddActionLog(Character, TargetCharacter, FText::FromString("zaatakowal"));
		}
	}
}

void AKKGameMode::PerformCharacterAbility(AKKCharacter* Character, AKKCharacter* TargetCharacter)
{
	if (RoundManager->CanUseCharacter(Character, EMP_AttackCharacter))
	{
		if (Character->ActiveAbility(TargetCharacter))
		{
			RoundManager->AddCharacterToList(Character, EMP_AttackCharacter);
			AddActionLog(Character, Character, FText::FromString("uzyl umiejetnosci " + Character->CharacterDataAsset->ActiveAbilities[0].AbilityName.ToString()));
		}
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
}

void AKKGameMode::ChangeTurn()
{
	Players[0]->bIsMyTurn = FirstPlayerTurn;
	Players[1]->bIsMyTurn = !FirstPlayerTurn;

	Players[0]->OnRep_TurnChanged();

	FirstPlayerTurn = !FirstPlayerTurn;
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

void AKKGameMode::LogAction(AKKCharacter* Character, AKKCharacter* TargetCharacter, FText Action) //static function, to call from anywhere
{
	if(AKKGameState* GameState =  Cast<AKKGameState>(UGameplayStatics::GetGameState(Character)))
	{
		if(AKKPlayerController* PlayerController = Character->OwningPlayer)
		{
			FString PlayerName = PlayerController->PlayerState->GetPlayerName();

			FText Log = FText::FormatOrdered(FTextFormat::FromString("{0}: {1}({2}) {3} {4}") , FText::FromString(PlayerName), Character->GetCharacterName(), Character->CharacterID , Action, (TargetCharacter ? TargetCharacter->GetCharacterName() : FText::FromString(" ")));

			GameState->AddActionLog(Log);
		}
	}
}