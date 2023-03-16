// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "CharacterStatsWidget.h"


// Sets default values
APlayerHUD::APlayerHUD()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}


// Called when the game starts or when spawned
void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwningPlayerController()->IsLocalController())
	{
		if(SelectedCharacterWidgetClass)
			SelectedCharacterWidget = CreateWidget<UCharacterStatsWidget>(GetOwningPlayerController(), SelectedCharacterWidgetClass);

		if(TargetCharacterWidgetClass)
			TargetCharacterWidget = CreateWidget<UCharacterStatsWidget>(GetOwningPlayerController(), TargetCharacterWidgetClass);
	}
	
}
