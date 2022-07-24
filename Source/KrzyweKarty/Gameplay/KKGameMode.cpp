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

void AKKGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AKKPlayerController* PlayerController = Cast<AKKPlayerController>(NewPlayer))
	{
		Players.Add(PlayerController);

		PlayerController->PlayerID = Players.Num();

		OnPlayerJoined.Broadcast();

		if (Players.Num() == 2)
		{
			ChangeTurn();
		}
	}
}

AActor* AKKGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);

	for (AActor* Point : FoundActors)
	{
		if (APlayerStart* StartPoint = Cast<APlayerStart>(Point))
		{
			if (StartPoint->PlayerStartTag.ToString() == FString::FromInt(Players.Num() + 1)) //happens before PostLogin()
			{
				return StartPoint;
			}
		}
	}
	return nullptr;
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
	if (RoundManager->CanUseCharacter(Character, EMP_AttackedCharacter))
	{
		if (Character->DefaultAttack(TargetCharacter))
		{
			RoundManager->AddCharacterToList(Character, EMP_AttackedCharacter);
		}
	}
}

void AKKGameMode::PerformCharacterAbility(AKKCharacter* Character, AKKCharacter* TargetCharacter)
{
	if (RoundManager->CanUseCharacter(Character, EMP_AttackedCharacter))
	{
		if (Character->ActiveAbility(TargetCharacter))
		{
			RoundManager->AddCharacterToList(Character, EMP_AttackedCharacter);
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

	FirstPlayerTurn = !FirstPlayerTurn;
}
