// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityHelperComponent.h"

#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKGameState.h"


// Sets default values for this component's properties
UAbilityHelperComponent::UAbilityHelperComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UAbilityHelperComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = GetOwner<AKKCharacter>();
}

AKKMap* UAbilityHelperComponent::GetMap()
{
	return GetWorld()->GetGameState<AKKGameState>()->Map;
}

AKKGameState* UAbilityHelperComponent::GetGameState()
{
	return GetWorld()->GetGameState<AKKGameState>();
}

AKKPlayerController* UAbilityHelperComponent::GetPlayerController()
{
	return OwnerCharacter->OwningPlayer;
}

