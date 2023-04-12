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
class UImage;

UCLASS()
class KRZYWEKARTY_API UCharacterStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void ShowStats(AKKCharacter* NewCharacter);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	AKKCharacter* Character;

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

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* DefenceImage;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* HealthImage;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* ManaImage;

public:
	const FText FormatText = FText::FromString("{0}");

	UFUNCTION(BlueprintPure)
	FText HealthText();

	UFUNCTION(BlueprintPure)
	FText ManaText();

	UFUNCTION(BlueprintPure)
	FText DefenceText();

	UFUNCTION(BlueprintPure)
	FText StrengthText();

	UFUNCTION()
	void RemoveCharacter();
};
