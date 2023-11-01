// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAbilityComponent.h"

#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKGameState.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"


// Sets default values for this component's properties
UCharacterAbilityComponent::UCharacterAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterAbilityComponent::OnBeginAbility_Implementation()
{
}

void UCharacterAbilityComponent::OnFinishAbility_Implementation()
{
	
}

void UCharacterAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = GetOwner<AKKCharacter>();
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

