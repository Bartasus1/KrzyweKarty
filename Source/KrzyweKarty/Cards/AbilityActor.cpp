// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActor.h"
#include "Action.h"
#include "KKCharacter.h"

#include "Net/UnrealNetwork.h"

AAbilityActor::AAbilityActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	AutoReceiveInput = EAutoReceiveInput::Player0;
}

void AAbilityActor::OnBeginAbility_Client_Implementation()
{
	OnBeginAbility();
}

void AAbilityActor::OnFinishAbility_Client_Implementation()
{
	OnFinishAbility();
}

void AAbilityActor::BeginAbility_Implementation()
{
	Character->AbilityActor = this;
	
	AbilityAction = NewObject<UAbilityAction>(this, "AbilityAction");
	AbilityAction->Index = AbilityIndex;
	AbilityAction->Character = Character;

	AbilityAction->TryBeginAction();
	OnBeginAbility_Client();
}

void AAbilityActor::ConfirmAbility_Implementation()
{
	AbilityAction->OnAbilityConfirmed();
	OnFinishAbility_Client();
	Destroy();
}

void AAbilityActor::AbortAbility_Implementation()
{
	OnFinishAbility_Client();
	Destroy();
}

void AAbilityActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAbilityActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAbilityActor, Character);
	DOREPLIFETIME(AAbilityActor, AbilityIndex);
}
