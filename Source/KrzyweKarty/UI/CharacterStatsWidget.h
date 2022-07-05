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

public:
	const FText FormatText = FText::FromString("{0}: {1} / {2}");

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
