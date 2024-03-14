// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActor.h"
#include "Action.h"
#include "KKCharacter.h"

AAbilityActor::AAbilityActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	AutoReceiveInput = EAutoReceiveInput::Player0;
}

void AAbilityActor::ConfirmAbility()
{
	AbilityAction->OnAbilityConfirmed();
	Destroy();
}

void AAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAbilityActor::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	Character = Cast<AKKCharacter>(NewOwner);
	
	AbilityAction = NewObject<UAbilityAction>(this, "AbilityAction");
	AbilityAction->Index = AbilityIndex;
	AbilityAction->Character = Character;

	AbilityAction->TryBeginAction();
}
