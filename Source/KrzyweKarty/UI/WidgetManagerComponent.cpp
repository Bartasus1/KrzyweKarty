// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManagerComponent.h"
#include "CharacterStatsWidget.h"

// Sets default values for this component's properties
UWidgetManagerComponent::UWidgetManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


// Called when the game starts
void UWidgetManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if(SelectedCharacterWidgetClass)
		SelectedCharacterWidget = CreateWidget<UCharacterStatsWidget>(GetOwner(), SelectedCharacterWidgetClass);

	if(TargetCharacterWidgetClass)
		TargetCharacterWidget = CreateWidget<UCharacterStatsWidget>(GetOwner(), TargetCharacterWidgetClass);

	if(ChangeTurnWidgetClass)
		ChangeTurnWidget = ChangeTurnWidget(GetOwner(), ChangeTurnWidgetClass);
	
}


