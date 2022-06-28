// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameMode.h"
#include "Kismet/GameplayStatics.h"

AKKGameMode::AKKGameMode()
{
}

void AKKGameMode::AddCharacterToMap(AKKCharacter* Character, int32 TileID)
{
	Map = Cast<AKKMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AKKMap::StaticClass()));
	
	Map->AddCharacterToMap(Character, TileID);
}

void AKKGameMode::MoveForward(AKKCharacter* Character, int32 ID)
{
	Map = Cast<AKKMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AKKMap::StaticClass()));

	if (ID == 1)
	{
		Map->MoveForward(Character);
	}
	else
	{
		Map->MoveBackward(Character);
	}
}

void AKKGameMode::MoveBackward(AKKCharacter* Character, int32 ID)
{
	Map = Cast<AKKMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AKKMap::StaticClass()));

	if (ID == 1)
	{
		Map->MoveBackward(Character);
	}
	else
	{
		Map->MoveForward(Character);
	}
}

void AKKGameMode::MoveRight(AKKCharacter* Character, int32 ID)
{
	Map = Cast<AKKMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AKKMap::StaticClass()));

	if (ID == 1)
	{
		Map->MoveRight(Character);
	}
	else
	{
		Map->MoveLeft(Character);
	}
}

void AKKGameMode::MoveLeft(AKKCharacter* Character, int32 ID)
{
	Map = Cast<AKKMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AKKMap::StaticClass()));

	if (ID == 1)
	{
		Map->MoveLeft(Character);
	}
	else
	{
		Map->MoveRight(Character);
	}
}

void AKKGameMode::BeginPlay()
{
	Super::BeginPlay();
}
