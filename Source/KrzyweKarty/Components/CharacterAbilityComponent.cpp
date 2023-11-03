// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAbilityComponent.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKGameState.h"


// Sets default values for this component's properties
UCharacterAbilityComponent::UCharacterAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);
}

void UCharacterAbilityComponent::OnBeginAbility_Implementation(int32 Index)
{
}

void UCharacterAbilityComponent::OnFinishAbility_Implementation(int32 Index)
{
	
}

void UCharacterAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = GetOwner<AKKCharacter>();
	
	OwnerCharacter->OnBeginAbilityDelegate.AddUniqueDynamic(this, &UCharacterAbilityComponent::OnBeginAbility);
	OwnerCharacter->OnFinishAbilityDelegate.AddUniqueDynamic(this, &UCharacterAbilityComponent::OnFinishAbility);
}

AKKMap* UCharacterAbilityComponent::GetMap() const
{
	return GetWorld()->GetGameState<AKKGameState>()->Map;
}

AKKGameState* UCharacterAbilityComponent::GetGameState() const
{
	return GetWorld()->GetGameState<AKKGameState>();
}

AKKPlayerController* UCharacterAbilityComponent::GetPlayerController() const
{
	return OwnerCharacter->OwningPlayer;
}

