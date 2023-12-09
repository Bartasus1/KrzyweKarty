// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaEffectCharacterAbilityComponent.h"

#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Interfaces/AreaModifierInterface.h"
#include "KrzyweKarty/Map/KKTile.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UAreaEffectCharacterAbilityComponent::UAreaEffectCharacterAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	// ...
}

void UAreaEffectCharacterAbilityComponent::OnBeginAbility_Implementation(int32 Index)
{
	Super::OnBeginAbility_Implementation(Index);

	if(OwnerCharacter->Implements<UAreaModifierInterface>())
	{
		Server_SetAffectedTiles(IAreaModifierInterface::Execute_GetAffectedTiles(OwnerCharacter, Index));

		SetComponentTickEnabled(true);
	}
}

void UAreaEffectCharacterAbilityComponent::OnFinishAbility_Implementation(int32 Index)
{
	Super::OnFinishAbility_Implementation(Index);

	GetMap()->ClearTilesHighlights();
	SetComponentTickEnabled(false);
}

void UAreaEffectCharacterAbilityComponent::Server_SetAffectedTiles_Implementation(const TArray<FDirection>& InAffectedTiles)
{
	AffectedTiles = InAffectedTiles;
}

const TArray<FDirection>& UAreaEffectCharacterAbilityComponent::GetFinalAffectedTiles() const
{
	return AffectedTiles;
}

void UAreaEffectCharacterAbilityComponent::RotateSelectedTiles(ERotationDirection RotationDirection)
{
	TArray<FDirection> Directions = IAreaModifierInterface::Execute_GetAffectedTiles(OwnerCharacter, AbilityIndex);
	for(FDirection& Direction : Directions)
	{
		Direction = Direction.Rotate(RotationDirection);
	}

	Server_SetAffectedTiles(Directions);
}


void UAreaEffectCharacterAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAreaEffectCharacterAbilityComponent, AffectedTiles);
}

void UAreaEffectCharacterAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetMap()->ClearTilesHighlights();

	for(AKKTile* Tile: GetMap()->GetTilesByDirection(OwnerCharacter, AffectedTiles, TileSelectionPolicy))
	{
		Tile->SetTileColor(TileColor);
	}
}

