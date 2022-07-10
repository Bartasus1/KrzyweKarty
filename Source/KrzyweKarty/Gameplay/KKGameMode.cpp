// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameMode.h"
#include "KKPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Map/KKMap.h"
#include "Kismet/GameplayStatics.h"

AKKGameMode::AKKGameMode()
{
}

void AKKGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AKKPlayerController* PlayerController = Cast<AKKPlayerController>(NewPlayer))
	{
		Players.Add(PlayerController);
		PlayerController->PlayerID = Players.Num();

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
			if (StartPoint->PlayerStartTag.ToString() == FString::FromInt(Players.Num() + 1))
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
		IncreaseMovesCounter();
	}
}

void AKKGameMode::MoveForward(AKKCharacter* Character, int32 PlayerID)
{
	if ((PlayerID == 1) ? Map->MoveForward(Character) : Map->MoveBackward(Character))
	{
		IncreaseMovesCounter();
	}
}

void AKKGameMode::MoveBackward(AKKCharacter* Character, int32 PlayerID)
{
	if ((PlayerID == 1) ? Map->MoveBackward(Character) : Map->MoveForward(Character))
	{
		IncreaseMovesCounter();
	}
}

void AKKGameMode::MoveRight(AKKCharacter* Character, int32 PlayerID)
{
	if ((PlayerID == 1) ? Map->MoveRight(Character) : Map->MoveLeft(Character))
	{
		IncreaseMovesCounter();
	}
}

void AKKGameMode::MoveLeft(AKKCharacter* Character, int32 PlayerID)
{
	if ((PlayerID == 1) ? Map->MoveLeft(Character) : Map->MoveRight(Character))
	{
		IncreaseMovesCounter();
	}
}

void AKKGameMode::EndGameWithWinner(int32 PlayerID)
{
	AKKPlayerController* Winner = Players[PlayerID - 1];
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

void AKKGameMode::IncreaseMovesCounter()
{
	MoveCounter++;

	if (MoveCounter >= 3)
	{
		MoveCounter = 0;
		ChangeTurn();
	}
}

void AKKGameMode::ChangeTurn()
{
	Players[0]->CanMove = FirstPlayerTurn;
	Players[1]->CanMove = !FirstPlayerTurn;

	FirstPlayerTurn = !FirstPlayerTurn;
}

