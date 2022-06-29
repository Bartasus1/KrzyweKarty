// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameMode.h"
#include "Kismet/GameplayStatics.h"

AKKGameMode::AKKGameMode()
{
}

bool AKKGameMode::AddCharacterToMap(AKKCharacter* Character, int32 TileID)
{
	Map = Cast<AKKMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AKKMap::StaticClass()));
	
	return Map->AddCharacterToMap(Character, TileID);
}

bool AKKGameMode::MoveForward(AKKCharacter* Character, int32 ID)
{
	Map = Cast<AKKMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AKKMap::StaticClass()));

	if (ID == 1)
	{
		return Map->MoveForward(Character);
	}
	else
	{
		return Map->MoveBackward(Character);
	}
}

bool AKKGameMode::MoveBackward(AKKCharacter* Character, int32 ID)
{
	Map = Cast<AKKMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AKKMap::StaticClass()));

	if (ID == 1)
	{
		return Map->MoveBackward(Character);
	}
	else
	{
		return Map->MoveForward(Character);
	}
}

bool AKKGameMode::MoveRight(AKKCharacter* Character, int32 ID)
{
	Map = Cast<AKKMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AKKMap::StaticClass()));

	if (ID == 1)
	{
		return Map->MoveRight(Character);
	}
	else
	{
		return Map->MoveLeft(Character);
	}
}

bool AKKGameMode::MoveLeft(AKKCharacter* Character, int32 ID)
{
	Map = Cast<AKKMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AKKMap::StaticClass()));

	if (ID == 1)
	{
		return Map->MoveLeft(Character);
	}
	else
	{
		return Map->MoveRight(Character);
	}
}

void AKKGameMode::BeginPlay()
{
	Super::BeginPlay();
}
