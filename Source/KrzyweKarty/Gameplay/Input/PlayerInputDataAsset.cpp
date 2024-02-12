// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInputDataAsset.h"

#include "EnhancedInputSubsystems.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"

UEnhancedInputLocalPlayerSubsystem* UPlayerInputDataAsset::GetEnhancedInputSubsystem(AKKPlayerController* PlayerController)
{
	return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
}

void UPlayerInputDataAsset::AddMappingContext(AKKPlayerController* PlayerController, UInputMappingContext* MappingContext, int32 Priority)
{
	GetEnhancedInputSubsystem(PlayerController)->AddMappingContext(MappingContext, INT32_MAX);
}

void UPlayerInputDataAsset::RemoveMappingContext(AKKPlayerController* PlayerController, UInputMappingContext* MappingContext)
{
	GetEnhancedInputSubsystem(PlayerController)->RemoveMappingContext(MappingContext);
}

void UPlayerInputDataAsset::RemoveMappingContext(AKKPlayerController* PlayerController, UInputMappingContext* UPlayerInputDataAsset::* MemberMappingContext)
{
	
	GetEnhancedInputSubsystem(PlayerController)->RemoveMappingContext(this->*MemberMappingContext);
}
