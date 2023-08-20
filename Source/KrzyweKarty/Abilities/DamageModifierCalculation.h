// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "DamageModifierCalculation.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API UDamageModifierCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

	UDamageModifierCalculation();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	UPROPERTY()
	FGameplayEffectAttributeCaptureDefinition SourceStrength;

	UPROPERTY()
	FGameplayEffectAttributeCaptureDefinition TargetDefence;
};
