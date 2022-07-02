// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameMode.h"
#include "KKPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "KrzyweKarty/Map/KKMap.h"
#include "Kismet/GameplayStatics.h"
#include "KrzyweKarty/Interfaces/BaseInterface.h"

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
		IncreaseCounter();
	}
}

void AKKGameMode::MoveForward(AKKCharacter* Character, int32 PlayerID)
{
	if ((PlayerID == 1) ? Map->MoveForward(Character) : Map->MoveBackward(Character))
	{
		IncreaseCounter();
	}
}

void AKKGameMode::MoveBackward(AKKCharacter* Character, int32 PlayerID)
{
	if ((PlayerID == 1) ? Map->MoveBackward(Character) : Map->MoveForward(Character))
	{
		IncreaseCounter();
	}
}

void AKKGameMode::MoveRight(AKKCharacter* Character, int32 PlayerID)
{
	if ((PlayerID == 1) ? Map->MoveRight(Character) : Map->MoveLeft(Character))
	{
		IncreaseCounter();
	}
}

void AKKGameMode::MoveLeft(AKKCharacter* Character, int32 PlayerID)
{
	if ((PlayerID == 1) ? Map->MoveLeft(Character) : Map->MoveRight(Character))
	{
		IncreaseCounter();
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

void AKKGameMode::IncreaseCounter()
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

int32 AKKGameMode::GetDistance(AKKCharacter* FirstCharacter, AKKCharacter* SecondCharacter)
{
	int32 TileOne = FirstCharacter->OwnedTileID;
	int32 TileTwo = SecondCharacter->OwnedTileID;
	
	if (SecondCharacter->Implements<UBaseInterface>())
	{
		if (TileOne == 1 || TileOne == 2 || TileOne == 17 || TileOne == 18)
		{
			return 0;
		}
	}

	FVector2D PositionOne = FVector2D(TileOne / 4, TileOne % 4);
	FVector2D PositionTwo = FVector2D(TileTwo / 4, TileTwo % 4);

	UE_LOG(LogTemp, Warning, TEXT("%d"), static_cast<int32>(FVector2D::Distance(PositionOne, PositionTwo)))

	return FVector2D::Distance(PositionOne, PositionTwo);
}
