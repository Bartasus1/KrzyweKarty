// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKGameState.h"

UAction::UAction()
{
}

void UAction::TryBeginAction()
{
	if(CanCharacterMakeAction())
	{
		BeginAction();
	}
}

void UAction::BeginAction()
{
}

int32 UAction::GetActionWeight()
{
	return ActionWeight;
}

bool UAction::WasActionSuccessful()
{
	return bWasActionSuccessful;
}

void UAction::AddActionToCharacterList() const
{
	Character->CharacterActions.AddUnique(this);
}

bool UAction::CanCharacterMakeAction() const
{
	if(Character->CharacterActions.Num() == 0)
		return true;
	if(Character->CharacterActions.Num() >= 3)
		return false;
	
	return (Character != nullptr && Character->CharacterActions.Top()->ActionWeight < ActionWeight);
}

AKKGameState* UAction::GetGameState() const
{
	return GetWorld()->GetGameState<AKKGameState>();
}
/////////////////////////////////////////////////////////////////////////////////////////////////

bool UMapAction::CanCharacterMakeAction() const
{
	return (Super::CanCharacterMakeAction() && GetMap()->GetCharacterAtIndex(DestinationTileID) == nullptr);
	
}

AKKMap* UMapAction::GetMap() const
{
return GetGameState()->Map;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

USummonAction::USummonAction()
{
	ActionWeight = 1;
}

bool USummonAction::CanCharacterMakeAction() const
{
	return (Super::CanCharacterMakeAction() && !Character->IsCharacterOnMap());
}

void USummonAction::BeginAction()
{
	GetMap()->AddCharacterToMap(Character, DestinationTileID);
	AddActionToCharacterList();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

UMoveAction::UMoveAction()
{
	ActionWeight = 2;
}

bool UMoveAction::CanCharacterMakeAction() const
{
	return (Super::CanCharacterMakeAction() && Character->IsCharacterOnMap());
}

void UMoveAction::BeginAction()
{
	GetMap()->MoveCharacter(Character, DestinationTileID);
	AddActionToCharacterList();
}

////////////////////////////////////////////////////////////////////////////////////////////////
UAttackAction::UAttackAction()
{
	ActionWeight = 3;
}

void UAttackAction::TryBeginAction()
{
	if(!CanCharacterMakeAction() || !Character->IsCharacterOnMap() || TargetCharacter == nullptr || TargetCharacter == Character)
	{
		return;
	}

	if(!Character->IsInTheSameTeam(TargetCharacter))
	{
		BeginAction();
	}
}

void UAttackAction::BeginAction()
{
	FAttackResultInfo AttackResultInfo = Character->DefaultAttack(TargetCharacter);

	if(AttackResultInfo.AttackStatus == EAttackResult::AttackConfirmed)
	{
		AddActionToCharacterList();
	}
}
