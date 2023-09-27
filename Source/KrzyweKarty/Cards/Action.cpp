// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKGameState.h"
#include "KrzyweKarty/Map/KKTile.h"
#include "KrzyweKarty/Subsystems/RoundManagerSubsystem.h"

UAction::UAction()
{
}

void UAction::TryBeginAction()
{
	if(CanCharacterMakeAction())
	{
		BeginAction();
		GetWorld()->GetSubsystem<URoundManagerSubsystem>()->RegisterCharacterInSystem(Character);
	}
}

void UAction::BeginAction()
{
}

void UAction::ShowActionAffectedTiles() const
{
}

int32 UAction::GetActionWeight() const
{
	return ActionWeight;
}

void UAction::AddActionToCharacterList() const
{
	Character->CharacterActions.AddUnique(ActionWeight);
}

bool UAction::CanCharacterMakeAction() const
{
	if(Character == nullptr)
	{
		return false;
	}
	
	return Character->GetTopActionWeight() < ActionWeight;
}

AKKGameState* UAction::GetGameState() const
{
	return GetWorld()->GetGameState<AKKGameState>();
}

AKKMap* UAction::GetMap() const
{
	return GetGameState()->Map;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
bool UMapAction::CanCharacterMakeAction() const
{
	return (Super::CanCharacterMakeAction() && GetMap()->GetCharacterAtIndex(DestinationTileID) == nullptr);
	
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

void USummonAction::ShowActionAffectedTiles() const
{
	for(AKKTile* Tile: GetMap()->GetTilesForSpawn(Character, Character->GetPossibleSpawnTiles()))
	{
		Tile->SetTileColor(Blue);
	}
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

void UMoveAction::ShowActionAffectedTiles() const
{
	if(Character->GetTopActionWeight() < ActionWeight)
	{
		for(AKKTile* Tile: Character->GetMoveTiles())
		{
			Tile->SetTileColor(Yellow);
		}
	}
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
	Character->DefaultAttack(TargetCharacter);
	
	AddActionToCharacterList();
}

void UAttackAction::ShowActionAffectedTiles() const
{
	if(Character->GetTopActionWeight() < ActionWeight)
	{
		for(AKKTile* Tile: Character->GetAttackTiles())
		{
			Tile->SetTileColor(Red);
		}
	}
}
