// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UCharacterStatsWidget;

UCLASS()
class KRZYWEKARTY_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerHUD();

	UPROPERTY(BlueprintReadWrite)
	UCharacterStatsWidget* SelectedCharacterWidget;

	UPROPERTY(BlueprintReadWrite)
	UCharacterStatsWidget* TargetCharacterWidget;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* ChangeTurnWidget;


	
	UFUNCTION(BlueprintImplementableEvent)
	void OnTurnChange(bool bIsMyTurn);

protected:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UCharacterStatsWidget> SelectedCharacterWidgetClass;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UCharacterStatsWidget> TargetCharacterWidgetClass;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
