// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStatsWidget.generated.h"

/**
 * 
 */
class AKKCharacter;
class UTextBlock;

UCLASS()
class KRZYWEKARTY_API UCharacterStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void ShowStats(AKKCharacter* Character);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CharacterNameText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CharacterHealthText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CharacterManaText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CharacterDefenceText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CharacterStrengthText;
};
