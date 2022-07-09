// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WidgetManagerComponent.generated.h"

class UCharacterStatsWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRZYWEKARTY_API UWidgetManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWidgetManagerComponent();
	
	UPROPERTY()
	UCharacterStatsWidget* SelectedCharacterWidget;

	UPROPERTY()
	UCharacterStatsWidget* TargetCharacterWidget;

	UPROPERTY()
	UUserWidget* ChangeTurnWidget;

protected:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UCharacterStatsWidget> SelectedCharacterWidgetClass;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UCharacterStatsWidget> TargetCharacterWidgetClass;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UCharacterStatsWidget> ChangeTurnWidgetClass;
	
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
