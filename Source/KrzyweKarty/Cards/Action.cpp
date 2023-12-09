// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKGameState.h"
#include "KrzyweKarty/Map/KKTile.h"

UAction::UAction()
{
}

void UAction::TryBeginAction()
{
	if(CanCharacterMakeAction())
	{
		BeginAction();
		AddActionToCharacterList();
		GetGameState()->RegisterCharacterInSystem(Character);
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

	if(bRequiresCharacterOnMap && !Character->IsCharacterOnMap())
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

//////////////////////////////////////////////////////////////////////////////////////////////////

USummonAction::USummonAction()
{
	ActionWeight = 1;
	bRequiresCharacterOnMap = false;
}

bool USummonAction::CanCharacterMakeAction() const
{
	return Super::CanCharacterMakeAction() && GetMap()->GetCharacterAtIndex(DestinationTileID) == nullptr;
}

void USummonAction::BeginAction()
{
	GetMap()->AddCharacterToMap(Character, DestinationTileID);
}

void USummonAction::ShowActionAffectedTiles() const
{
	for(AKKTile* Tile: GetMap()->GetTilesForSpawn(Character, Character->GetPossibleSpawnTiles()))
	{
		Tile->SetTileColor(ETileColor::Blue);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

UMoveAction::UMoveAction()
{
	ActionWeight = 2;
	bRequiresCharacterOnMap = true;
}

bool UMoveAction::CanCharacterMakeAction() const
{
	return Super::CanCharacterMakeAction() && GetMap()->GetCharacterAtIndex(DestinationTileID) == nullptr;
}

void UMoveAction::BeginAction()
{
	GetMap()->MoveCharacter(Character, DestinationTileID);
}

void UMoveAction::ShowActionAffectedTiles() const
{
	if(Character->GetTopActionWeight() < ActionWeight)
	{
		for(AKKTile* Tile: Character->GetMoveTiles())
		{
			Tile->SetTileColor(ETileColor::Yellow);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
UAttackAction::UAttackAction()
{
	ActionWeight = 3;
	bRequiresCharacterOnMap = true;
}

bool UAttackAction::CanCharacterMakeAction() const
{
	if(Super::CanCharacterMakeAction() == false && TargetCharacter == nullptr)
	{
		return false;
	}

	const bool bAreBothCharactersOnMap = TargetCharacter->IsCharacterOnMap() && Character->IsCharacterOnMap();
	const bool bIsTargetCharacterValid = TargetCharacter != Character && !Character->IsInTheSameTeam(TargetCharacter);
	
	return bIsTargetCharacterValid && bAreBothCharactersOnMap;
}

void UAttackAction::BeginAction()
{
	Character->DefaultAttack(TargetCharacter);
}

void UAttackAction::ShowActionAffectedTiles() const
{
	if(Character->GetTopActionWeight() < ActionWeight)
	{
		for(AKKTile* Tile: Character->GetAttackTiles())
		{
			Tile->SetTileColor(ETileColor::Red);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////

UAbilityAction::UAbilityAction()
{
	ActionWeight = 3;
	bRequiresCharacterOnMap = true;
}

void UAbilityAction::OnAbilityConfirmed()
{
	if(Character->CanFinishAbility(Index))
	{
		Character->PerformAbility(Index);
		Character->CommitAbilityCost(Index);

		Character->OnFinishAbility(Index);
	
		AddActionToCharacterList();
		GetGameState()->RegisterCharacterInSystem(Character);
	}
}

void UAbilityAction::OnAbilityAborted()
{
	Character->OnFinishAbility(Index);
}

void UAbilityAction::TryBeginAction()
{
	if(CanCharacterMakeAction())
	{
		BeginAction();
		// don't add action to character's action list yet
	}
}

bool UAbilityAction::CanCharacterMakeAction() const
{
	return Super::CanCharacterMakeAction() && Character->CanUseAbility(Index);
}

void UAbilityAction::BeginAction()
{
	Character->OnBeginAbility(Index);
}
