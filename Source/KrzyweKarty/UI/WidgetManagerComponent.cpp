// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManagerComponent.h"
#include "CharacterStatsWidget.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"

// Sets default values for this component's properties
UWidgetManagerComponent::UWidgetManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


// Called when the game starts
void UWidgetManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	AKKPlayerController* PlayerController = Cast<AKKPlayerController>(GetOwner());

	if(PlayerController->IsLocalController())
	{
		if(SelectedCharacterWidgetClass)
			SelectedCharacterWidget = CreateWidget<UCharacterStatsWidget>(Cast<AKKPlayerController>(GetOwner()), SelectedCharacterWidgetClass);

		if(TargetCharacterWidgetClass)
			TargetCharacterWidget = CreateWidget<UCharacterStatsWidget>(Cast<AKKPlayerController>(GetOwner()), TargetCharacterWidgetClass);

		if(ChangeTurnWidgetClass)
			ChangeTurnWidget = CreateWidget(Cast<AKKPlayerController>(GetOwner()), ChangeTurnWidgetClass);
	}
}


