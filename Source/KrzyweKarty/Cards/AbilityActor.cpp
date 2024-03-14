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

void AAbilityActor::OnBeginAbility_Implementation()
{
}

void AAbilityActor::OnFinishAbility_Implementation()
{
}

void AAbilityActor::BeginAbility_Implementation()
{
	AbilityAction = NewObject<UAbilityAction>(this, "AbilityAction");
	AbilityAction->Index = AbilityIndex;
	AbilityAction->Character = Character;

	AbilityAction->TryBeginAction();
	OnBeginAbility();
}

void AAbilityActor::ConfirmAbility_Implementation()
{
	AbilityAction->OnAbilityConfirmed();
	OnFinishAbility();
	Destroy();
}

void AAbilityActor::AbortAbility_Implementation()
{
	OnFinishAbility();
	Destroy();
}

void AAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	
}
