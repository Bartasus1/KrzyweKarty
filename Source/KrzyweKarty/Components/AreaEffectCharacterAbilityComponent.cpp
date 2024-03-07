﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaEffectCharacterAbilityComponent.h"

#include "EnhancedInputComponent.h"
#include "KrzyweKarty/KKGameStatics.h"
#include "KrzyweKarty/TileStatusSettings.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"
#include "KrzyweKarty/Gameplay/Input/PlayerInputDataAsset.h"
#include "KrzyweKarty/Interfaces/AreaModifierInterface.h"
#include "KrzyweKarty/Map/KKTile.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UAreaEffectCharacterAbilityComponent::UAreaEffectCharacterAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	//PrimaryComponentTick.bStartWithTickEnabled = false;

}

void UAreaEffectCharacterAbilityComponent::OnBeginAbility_Implementation(uint8 Index)
{
	Super::OnBeginAbility_Implementation(Index);
	
	Server_SetAffectedTiles(IAreaModifierInterface::Execute_GetAffectedTiles(OwnerCharacter, Index));
	ShowAreaTiles();

	AKKPlayerController* PlayerController = UKKGameStatics::GetKKPlayerController(this);
	UPlayerInputDataAsset* PlayerInput = PlayerController->PlayerInputDataAsset;

	UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	InputActionBinding = &InputComponent->BindAction(PlayerInput->RotateDirections, ETriggerEvent::Triggered, this, &UAreaEffectCharacterAbilityComponent::RotateSelectedTiles);
}

void UAreaEffectCharacterAbilityComponent::OnFinishAbility_Implementation(uint8 Index)
{
	Super::OnFinishAbility_Implementation(Index);

	GetMap()->ClearTilesHighlights();
	AKKPlayerController* PlayerController = UKKGameStatics::GetKKPlayerController(this);
	UPlayerInputDataAsset* PlayerInput = PlayerController->PlayerInputDataAsset;

	UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	InputComponent->RemoveBinding(*InputActionBinding);
}

void UAreaEffectCharacterAbilityComponent::Server_SetAffectedTiles_Implementation(const TArray<FDirection>& InAffectedTiles)
{
	AffectedTiles = InAffectedTiles;
}

const TArray<FDirection>& UAreaEffectCharacterAbilityComponent::GetFinalAffectedTiles() const
{
	return AffectedTiles;
}

void UAreaEffectCharacterAbilityComponent::RotateSelectedTiles(const FInputActionInstance& InputAction)
{
	int32 Value = InputAction.GetValue().Get<float>();
	const ERotationDirection RotationDirection = static_cast<ERotationDirection>(Value);
	
	TArray<FDirection> Directions = IAreaModifierInterface::Execute_GetAffectedTiles(OwnerCharacter, AbilityIndex);
	for(FDirection& Direction : Directions)
	{
		Direction = Direction.Rotate(RotationDirection);
	}

	Server_SetAffectedTiles(Directions);

	GetMap()->ClearTilesHighlights();
	ShowAreaTiles();
}

void UAreaEffectCharacterAbilityComponent::ShowAreaTiles_Implementation()
{
	for(AKKTile* Tile: GetMap()->GetTilesByDirection(OwnerCharacter, AffectedTiles, TileSelectionPolicy))
 	{
 		Tile->SetTileStatus(UTileStatusSettings::GetDataAsset()->AttackTileStatus); //todo: make special tile status for ability tiles
 	}
}

void UAreaEffectCharacterAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAreaEffectCharacterAbilityComponent, AffectedTiles);
}


