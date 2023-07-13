// Fill out your copyright notice in the Description page of Project Settings.


#include "KKPlayerController.h"
#include "KKGameMode.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Interfaces/SelectableInterface.h"
#include "KrzyweKarty/UI/PlayerHUD.h"
#include "Net/UnrealNetwork.h"

AKKPlayerController::AKKPlayerController()
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
	
}

void AKKPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(AKKCharacter* TracedCharacter = Cast<AKKCharacter>(TraceForSelectable().GetObject()))
	{
		ShowTargetStats(TracedCharacter);
	}
}


void AKKPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKPlayerController, PlayerID);
	DOREPLIFETIME(AKKPlayerController, bIsMyTurn);
}

bool AKKPlayerController::MinMoveRequirements()
{
	return (SelectedCharacter != nullptr) && bIsMyTurn;
}

FHitResult AKKPlayerController::CastLineTrace(ECollisionChannel CollisionChannel) const
{
	float Range = 3500;
 	FVector Start, Direction;
 	DeprojectMousePositionToWorld(Start, Direction);
 
 	FHitResult HitResult;
 	FVector End = Start + (Direction * Range);
 
 	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, CollisionChannel);
	
	return HitResult;
}

TScriptInterface<ISelectableInterface> AKKPlayerController::TraceForSelectable(bool bHigherPriority) const
{
	return CastLineTrace((bHigherPriority ? PriorityTraceChannel : SelectableTraceChannel)).GetActor();
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
