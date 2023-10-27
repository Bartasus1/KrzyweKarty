// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaEffectAbilityComponent.h"

#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Interfaces/AreaModifierInterface.h"
#include "KrzyweKarty/Map/KKTile.h"


// Sets default values for this component's properties
UAreaEffectAbilityComponent::UAreaEffectAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UAreaEffectAbilityComponent::BeginAbility_Implementation()
{
	Super::BeginAbility_Implementation();

	AffectedTiles = GetMap()->GetTilesByDirection(OwnerCharacter, IAreaModifierInterface::Execute_GetAffectedTiles(OwnerCharacter, AbilityIndex));
	SetComponentTickEnabled(true);
}

void UAreaEffectAbilityComponent::FinishAbility_Implementation(bool bWasCancelled)
{
	Super::FinishAbility_Implementation(bWasCancelled);
	
	SetComponentTickEnabled(false);
	GetMap()->ClearTilesHighlights();
}

void UAreaEffectAbilityComponent::RotateArea(ERotationDirection RotationDirection)
{
	TArray<FDirection> AbilityAffectedTiles = IAreaModifierInterface::Execute_GetAffectedTiles(OwnerCharacter, AbilityIndex);

	for(FDirection& Direction : AbilityAffectedTiles)
	{
		Direction = Direction.Rotate(RotationDirection);
	}
	
	AffectedTiles = GetMap()->GetTilesByDirection(OwnerCharacter, AbilityAffectedTiles);
}

void UAreaEffectAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetComponentTickEnabled(false);
}

void UAreaEffectAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetMap()->ClearTilesHighlights();
	
	for(AKKTile* Tile: AffectedTiles)
	{
		Tile->SetTileColor(Red);
	}
}
