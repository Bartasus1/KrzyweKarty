// Fill out your copyright notice in the Description page of Project Settings.


#include "KKPlayerController.h"
#include "KrzyweKarty/KrzyweKarty.h"
#include "KrzyweKarty/Cards/Action.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Interfaces/SelectableInterface.h"
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

	if(IsLocalPlayerController())
	{
		CharacterActions.Add(USummonAction::StaticClass(), NewObject<USummonAction>(this, "Summon Action"));
		CharacterActions.Add(UMoveAction::StaticClass(), NewObject<UMoveAction>(this, "Move Action"));
		CharacterActions.Add(UAttackAction::StaticClass(), NewObject<UAttackAction>(this, "Attack Action"));
	}
}

void AKKPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(const TScriptInterface<ISelectableInterface> TracedSelectable = TraceForSelectable())
	{
		if(CachedTracedSelectableInterface != TracedSelectable)
		{
			if(CachedTracedSelectableInterface != nullptr)
			{
				CachedTracedSelectableInterface->OnSelectableLostFocus();
			}
			
			CachedTracedSelectableInterface = TracedSelectable;
			CachedTracedSelectableInterface->OnSelectableGainFocus();
		}

		if(AKKCharacter* TracedCharacter = Cast<AKKCharacter>(TracedSelectable.GetObject()))
		{
			ShowTargetStats(TracedCharacter);
		}
		
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

TScriptInterface<ISelectableInterface> AKKPlayerController::TraceForSelectable(bool bHigherPriority /* = false */) const
{
	return CastLineTrace((bHigherPriority ? PriorityTraceChannel : SelectableTraceChannel)).GetActor();
}

void AKKPlayerController::UpdateCharacterInActions()
{
	for(auto& Action: CharacterActions)
	{
		Action.Value->Character = SelectedCharacter;
	}
}

bool AKKPlayerController::SelectCharacter()
{
	if(AKKCharacter* TracedCharacter = Cast<AKKCharacter>(TraceForSelectable().GetObject()))
	{
		if(TracedCharacter->OwningPlayer == this)
		{
			SelectedCharacter = TracedCharacter;
			ShowCharacterStats(SelectedCharacter);
			
			return bIsMyTurn; //dont use character if its not my turn
		}
	}

	return false;
}

void AKKPlayerController::OnCharacterSelection()
{
	UpdateCharacterInActions();
	
	if(SelectedCharacter->IsCharacterOnMap())
	{
		CharacterActions[UMoveAction::StaticClass()]->ShowActionAffectedTiles();
		CharacterActions[UAttackAction::StaticClass()]->ShowActionAffectedTiles();
	}
	else
	{
		CharacterActions[USummonAction::StaticClass()]->ShowActionAffectedTiles();
	}
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
