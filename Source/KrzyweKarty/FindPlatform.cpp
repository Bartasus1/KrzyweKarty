// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlatform.h"

#include "Gameplay/KKPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Map/KKTile.h"

UFindPlatform* UFindPlatform::FindPlatform(const UObject* WorldContextObject, AKKPlayerController* PlayerController, UInputMappingContext* MappingContext, UInputAction* ClickAction)
{
	UFindPlatform* FindPlatform = NewObject<UFindPlatform>();
	FindPlatform->PlayerController = PlayerController;
	FindPlatform->MappingContext = MappingContext;
	FindPlatform->ClickAction = ClickAction;

	return FindPlatform;
}

void UFindPlatform::Activate()
{
	Super::Activate();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->AddMappingContext(MappingContext, INT32_MAX);

	UEnhancedInputComponent* InputComponent =  Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

	InputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &UFindPlatform::LookForPlatform);
	
}

void UFindPlatform::LookForPlatform()
{
	AKKTile* Tile = Cast<AKKTile>(PlayerController->TraceForSelectable().GetObject());

	if(Tile)
	{
		PlatformFound.Broadcast(Tile);

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		Subsystem->RemoveMappingContext(MappingContext);
	}
}
