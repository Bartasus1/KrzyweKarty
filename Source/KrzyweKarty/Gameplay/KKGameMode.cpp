// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameMode.h"
#include "KKPlayerController.h"
#include "RoundManager.h"
#include "GameFramework/PlayerStart.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Map/KKMap.h"
#include "Kismet/GameplayStatics.h"

AKKGameMode::AKKGameMode()
{
	RoundManager = CreateDefaultSubobject<URoundManager>("RoundManager");

	RoundManager->OnRoundEnd.AddUniqueDynamic(this, &AKKGameMode::ChangeTurn);
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

		if (Players.Num() == 2)
		{
			ChangeTurn();
		}
	}

	return PlayerController;
}

void AKKGameMode::AddCharacterToMap(AKKCharacter* Character, int32 TileID, int32 PlayerID)
{
	TileID = (PlayerID == 1) ? TileID : 19 - TileID;

	if (Map->AddCharacterToMap(Character, TileID))
	{
		RoundManager->AddCharacterToList(Character, EMP_SummonedCharacter);
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
	Players[0]->IsMyTurn = FirstPlayerTurn;
	Players[1]->IsMyTurn = !FirstPlayerTurn;

	Players[0]->OnRep_TurnChanged();
	Players[1]->OnRep_TurnChanged();

	FirstPlayerTurn = !FirstPlayerTurn;
}
