// Fill out your copyright notice in the Description page of Project Settings.


#include "KKGameStatics.h"
#include "Cards/KKCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Gameplay/KKGameMode.h"
#include "Gameplay/KKGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Map/KKMap.h"
#include "Map/KKTile.h"

AKKPlayerController* UKKGameStatics::GetKKPlayerController(const UObject* WorldContextObject)
{
	return Cast<AKKPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
}

AKKMap* UKKGameStatics::GetMap(const UObject* WorldContextObject)
{
	return WorldContextObject->GetWorld()->GetGameState<AKKGameState>()->Map;
}

void UKKGameStatics::AddActionLog( AKKCharacter* Character, AKKCharacter* TargetCharacter, FText Action)
{
	if(AKKGameState* GameState =  Cast<AKKGameState>(UGameplayStatics::GetGameState(Character)))
	{
		if(AKKPlayerController* PlayerController = Character->OwningPlayer)
		{
			FString PlayerName = PlayerController->PlayerState->GetPlayerName();

			FText Log = FText::FormatOrdered(FTextFormat::FromString("{0}: {1}({2}) {3} {4}") , FText::FromString(PlayerName), Character->GetCharacterName(), Character->CharacterID , Action, (TargetCharacter ? TargetCharacter->GetCharacterName() : FText::FromString(" ")));

			GameState->Server_AddActionLog(Log);
		}
	}
}

void UKKGameStatics::RotateDirections(IN TArray<FDirection>& Directions, ERotationDirection RotationDirection)
{
	for(FDirection& InDirection : Directions)
	{
		InDirection = InDirection.Rotate(RotationDirection);
	}
}

void UKKGameStatics::ShowTiles(TArray<AKKTile*> Tiles, ETileColor Color)
{
	for(auto& Tile : Tiles)
	{
		Tile->SetTileColor(Color);
	}
}

void UKKGameStatics::HideTiles(const UObject* WorldContextObject)
{
	GetMap(WorldContextObject)->ClearTilesHighlights();
}