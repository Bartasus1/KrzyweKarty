// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "KKCharacter.h"
#include "GameFramework/PlayerState.h"
#include "KrzyweKarty/TileStatusSettings.h"
#include "KrzyweKarty/Gameplay/KKGameState.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"
#include "KrzyweKarty/Interfaces/BaseInterface.h"
#include "KrzyweKarty/Map/KKMap.h"
#include "KrzyweKarty/Map/KKTile.h"

UAction::UAction()
{
}

void UAction::TryBeginAction()
{
	if(CanCharacterMakeAction())
	{
		BeginAction();
		OnActionCompleted();
	}
}

void UAction::OnActionCompleted()
{
	AddActionToCharacterList();
	GetGameState()->RegisterCharacterInSystem(Character);

	FString PlayerName = Character->OwningPlayer->PlayerState->GetPlayerName();
	FString Log = "<PlayerName>" + PlayerName + ":</> " + GetLogMessage();
	GetGameState()->Server_AddActionLog(FText::FromString(Log));
		
	ConditionalBeginDestroy();
}

void UAction::BeginAction()
{
}

TArray<AKKTile*> UAction::GetActionAffectedTiles() const
{
	return TArray<AKKTile*>();
}

UTileStatus* UAction::GetActionTileStatus() const
{
	return nullptr;
}

void UAction::ShowActionAffectedTiles() const
{
	if(Character->GetTopActionWeight() < ActionWeight)
	{
		for (AKKTile* Tile : GetActionAffectedTiles())
		{
			Tile->SetTileStatus(GetActionTileStatus());
		}
	}
}

bool UAction::ShouldShowTiles() const
{
	return Character->IsCharacterOnMap() && Character->GetTopActionWeight() < ActionWeight;
}

FString UAction::GetLogMessage()
{
	return FString();
}

uint8 UAction::GetActionWeight() const
{
	return ActionWeight;
}

bool UAction::RequiresCharacterOnMap() const
{
	return bRequiresCharacterOnMap;
}

void UAction::AddActionToCharacterList() const
{
	if(Character->CharacterActions < ActionWeight)
	{
		Character->CharacterActions = ActionWeight;
	}
	
}

bool UAction::CanCharacterMakeAction() const
{
	if(Character == nullptr || Character->Implements<UBaseInterface>())
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

TArray<AKKTile*> USummonAction::GetActionAffectedTiles() const
{
	return GetMap()->GetTilesForSpawn(Character, Character->GetPossibleSpawnTiles());
}

UTileStatus* USummonAction::GetActionTileStatus() const
{
	return UTileStatusSettings::GetDataAsset()->SummonTileStatus;
}

bool USummonAction::ShouldShowTiles() const
{
	return !Character->IsCharacterOnMap() && Character->GetTopActionWeight() < ActionWeight;
}

FString USummonAction::GetLogMessage()
{
	return "Summoned <Ally>" + Character->GetCharacterName().ToString() + "</>";
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

TArray<AKKTile*> UMoveAction::GetActionAffectedTiles() const
{
	return Character->GetMoveTiles();
}

UTileStatus* UMoveAction::GetActionTileStatus() const
{
	return UTileStatusSettings::GetDataAsset()->MovementTileStatus;
}

FString UMoveAction::GetLogMessage()
{
	return "Moved <Ally>" + Character->GetCharacterName().ToString() + "</>";
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

TArray<AKKTile*> UAttackAction::GetActionAffectedTiles() const
{
	TArray<AKKTile*> AttackTiles = GetMap()->GetTilesByDirection(Character, Character->GetPossibleAttackTiles(), TSP_EnemyCharactersOnly, true);

	if(GetMap()->CanAttackBase(Character))
	{
		GetMap()->GetTilesForBaseAttack(Character, AttackTiles);
	}

	return AttackTiles;
}

UTileStatus* UAttackAction::GetActionTileStatus() const
{
	return UTileStatusSettings::GetDataAsset()->AttackTileStatus;
}

FString UAttackAction::GetLogMessage()
{
	return "<Ally>" + Character->GetCharacterName().ToString() + "</> attacked <Enemy>" + TargetCharacter->GetCharacterName().ToString() + "</>";
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
	
		OnActionCompleted();
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

FString UAbilityAction::GetLogMessage()
{
	return Character->GetCharacterName().ToString() + " used ability <Ability>" + Character->CharacterDataAsset->ActiveAbilities[Index].AbilityName.ToString() + "</>";
}
