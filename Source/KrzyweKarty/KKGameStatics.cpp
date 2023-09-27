// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameStatics.h"
#include "Cards/KKCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Gameplay/KKGameMode.h"
#include "Gameplay/KKGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Map/KKMap.h"

int32 UKKGameStatics::GetMapSize(const UObject* WorldContextObject)
{
	if(AKKGameMode* GameMode = Cast<AKKGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return GameMode->GetMap()->GetMapSize();
	}
	return 0;
}

void UKKGameStatics::AddActionLog( AKKCharacter* Character, AKKCharacter* TargetCharacter, FText Action)
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

TArray<FDirection> UKKGameStatics::RotateDirections(TArray<FDirection> Directions, ERotationDirection RotationDirection)
{
	for(FDirection& InDirection : Directions)
	{
		InDirection = InDirection.Rotate(RotationDirection);
	}

	return Directions;
}
