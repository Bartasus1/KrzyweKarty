// Fill out your copyright notice in the Description page of Project Settings.


#include "FindSelectableAsync.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Gameplay/KKPlayerController.h"
#include "Gameplay/Input/PlayerInputDataAsset.h"

UFindSelectableAsync* UFindSelectableAsync::FindSelectableAsync(const UObject* WorldContextObject, AKKPlayerController* PlayerController, bool bTraceWithHigherPriority)
{
	UFindSelectableAsync* FindSelectableAsync = NewObject<UFindSelectableAsync>();
	FindSelectableAsync->PlayerController = PlayerController;
	FindSelectableAsync->bTraceWithHigherPriority = bTraceWithHigherPriority;

	return FindSelectableAsync;
}

void UFindSelectableAsync::Activate()
{
	Super::Activate();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	UPlayerInputDataAsset* PlayerInputDataAsset = PlayerController->PlayerInputDataAsset;
	
	Subsystem->AddMappingContext(PlayerInputDataAsset->SelectionMappingContext, INT32_MAX);

	UEnhancedInputComponent* InputComponent =  Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

	InputComponent->BindAction(PlayerInputDataAsset->FindSelectable, ETriggerEvent::Triggered, this, &UFindSelectableAsync::FindSelectable);
	InputComponent->BindAction(PlayerInputDataAsset->FindSelectableCancel, ETriggerEvent::Triggered, this, &UFindSelectableAsync::ReportFail);
	
}

void UFindSelectableAsync::FindSelectable()
{
	if(TScriptInterface<ISelectableInterface> SelectableInterface = PlayerController->TraceForSelectable(bTraceWithHigherPriority))
	{
		SelectableFound.Broadcast(SelectableInterface);

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		UPlayerInputDataAsset* PlayerInputDataAsset = PlayerController->PlayerInputDataAsset;
		
		Subsystem->RemoveMappingContext(PlayerInputDataAsset->SelectionMappingContext);
	}
	else
	{
		ReportFail();
	}
}

void UFindSelectableAsync::ReportFail()
{
	SelectableNotFound.Broadcast();
		
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	UPlayerInputDataAsset* PlayerInputDataAsset = PlayerController->PlayerInputDataAsset;
		
	Subsystem->RemoveMappingContext(PlayerInputDataAsset->SelectionMappingContext);
}
