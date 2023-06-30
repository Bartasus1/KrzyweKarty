// Fill out your copyright notice in the Description page of Project Settings.


#include "FindSelectableAsync.h"

#include "Gameplay/KKPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

UFindSelectableAsync* UFindSelectableAsync::FindSelectableAsync(const UObject* WorldContextObject, AKKPlayerController* PlayerController, UInputMappingContext* MappingContext, UInputAction* ClickAction, bool bTraceWithHigherPriority)
{
	UFindSelectableAsync* FindSelectableAsync = NewObject<UFindSelectableAsync>();
	FindSelectableAsync->PlayerController = PlayerController;
	FindSelectableAsync->MappingContext = MappingContext;
	FindSelectableAsync->ClickAction = ClickAction;
	FindSelectableAsync->bTraceWithHigherPriority = bTraceWithHigherPriority;

	return FindSelectableAsync;
}

void UFindSelectableAsync::Activate()
{
	Super::Activate();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->AddMappingContext(MappingContext, INT32_MAX);

	UEnhancedInputComponent* InputComponent =  Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

	InputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &UFindSelectableAsync::FindSelectable);
	
}

void UFindSelectableAsync::FindSelectable()
{
	if(TScriptInterface<ISelectableInterface> SelectableInterface = PlayerController->TraceForSelectable(bTraceWithHigherPriority))
	{
		SelectableFound.Broadcast(SelectableInterface);

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		Subsystem->RemoveMappingContext(MappingContext);
	}
	else
	{
		SelectableNotFound.Broadcast();
	}
}
