// Fill out your copyright notice in the Description page of Project Settings.


#include "KKPlayerController.h"
#include "KKGameState.h"

#include "Kismet/KismetSystemLibrary.h"

#include "KrzyweKarty/KrzyweKarty.h"
#include "KrzyweKarty/Cards/Action.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Interfaces/SelectableInterface.h"
#include "KrzyweKarty/Map/KKTile.h"
#include "KrzyweKarty/UI/PlayerHUD.h"
#include "Net/UnrealNetwork.h"

AKKPlayerController::AKKPlayerController(): PlayerInputDataAsset(nullptr)
{
	bReplicates = true;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AKKPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());

	PlayerCameraManager->ViewPitchMin = -70;
	PlayerCameraManager->ViewPitchMax = 10;


	if(HasAuthority())
	{
		CharacterActions.Add(USummonAction::StaticClass(), NewObject<USummonAction>(this, "Summon Action"));
		CharacterActions.Add(UMoveAction::StaticClass(), NewObject<UMoveAction>(this, "Move Action"));
		CharacterActions.Add(UAttackAction::StaticClass(), NewObject<UAttackAction>(this, "Attack Action"));
	}
}

void AKKPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(AKKCharacter* TracedCharacter = TraceForCharacter())
	{
		ShowTargetStats(TracedCharacter);
	}
	
}

void AKKPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKPlayerController, PlayerID);
	DOREPLIFETIME(AKKPlayerController, bIsMyTurn);
	DOREPLIFETIME(AKKPlayerController, SelectedCharacter);
}

bool AKKPlayerController::MinMoveRequirements() const
{
	return (SelectedCharacter != nullptr) && bIsMyTurn;
}

AKKMap* AKKPlayerController::GetMap() const
{
	return GetWorld()->GetGameState<AKKGameState>()->Map;
}

FHitResult AKKPlayerController::CastLineTrace(ECollisionChannel CollisionChannel) const
{
	float Range = 3500;
 	FVector Start, Direction;
 	DeprojectMousePositionToWorld(Start, Direction);
 
 	FHitResult HitResult;
 	FVector End = Start + (Direction * Range);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(SelectedCharacter); // todo: maybe make it a parameter 
	
 	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, CollisionChannel, QueryParams);
	//uint8 TraceChannel = CollisionChannel - 14;
	// TraceTypeQuery = static_cast<ETraceTypeQuery>((TraceChannel + 2));
	//UKismetSystemLibrary::LineTraceSingle(this, Start, End, TraceTypeQuery, false, {SelectedCharacter}, EDrawDebugTrace::ForDuration, HitResult, true);
	
	return HitResult;
}

TScriptInterface<ISelectableInterface> AKKPlayerController::TraceForCollisionChannel(ECollisionChannel CollisionChannel) const
{
	return CastLineTrace(CollisionChannel).GetActor();
}

AKKCharacter* AKKPlayerController::TraceForCharacter() const
{
	return Cast<AKKCharacter>(CastLineTrace(CharacterTraceChannel).GetActor());
}

AKKCharacter* AKKPlayerController::SelectCharacter()
{
	if(AKKCharacter* TracedCharacter = TraceForCharacter())
	{
		if(TracedCharacter->OwningPlayer == this)
		{
			SelectedCharacter = TracedCharacter;
			ShowCharacterStats(SelectedCharacter);
		}
	}

	return SelectedCharacter;
}


void AKKPlayerController::ShowCharacterStats(AKKCharacter* CardCharacter)
{
	GetHUD<APlayerHUD>()->ShowCharacterStats(CardCharacter, true);
}

void AKKPlayerController::ShowTargetStats(AKKCharacter* CardCharacter)
{
	GetHUD<APlayerHUD>()->ShowCharacterStats(CardCharacter, false);
}

void AKKPlayerController::OnRep_TurnChanged()
{
	if(GetHUD())
	{
		GetHUD<APlayerHUD>()->OnTurnChange(bIsMyTurn);
	}
}

void AKKPlayerController::OnCharacterSelection_Server_Implementation(AKKCharacter* InCharacter)
{
	SelectedCharacter = InCharacter; //assign on server
	
	if(bIsMyTurn && SelectedCharacter)
	{
		for (const auto& CharacterAction : CharacterActions)
		{
			UAction* Action = CharacterAction.Value;
			
			Action->Character = SelectedCharacter;

			if(Action->ShouldShowTiles())
			{
				ShowTilesForAction(Action->GetActionAffectedTiles(), Action->GetActionTileStatus());
			}
		}
	}
}

void AKKPlayerController::ShowTilesForAction_Implementation(const TArray<AKKTile*>& InTiles, UTileStatus* TileStatus)
{
	for (AKKTile* Tile : InTiles)
	{
		Tile->SetTileStatus(TileStatus);
	}
}
