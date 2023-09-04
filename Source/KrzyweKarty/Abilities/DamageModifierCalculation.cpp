// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageModifierCalculation.h"
#include "CharacterAttributeSet.h"

UDamageModifierCalculation::UDamageModifierCalculation()
{
	TargetDefence.AttributeToCapture = UCharacterAttributeSet::GetDefenceAttribute();
	TargetDefence.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	TargetDefence.bSnapshot = true;

	SourceStrength.AttributeToCapture = UCharacterAttributeSet::GetStrengthAttribute();
	SourceStrength.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	SourceStrength.bSnapshot = true;
	
	RelevantAttributesToCapture.Add(TargetDefence);
	RelevantAttributesToCapture.Add(SourceStrength);
}

float UDamageModifierCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Defence = 0.f;
    GetCapturedAttributeMagnitude(TargetDefence, Spec, EvaluationParameters, Defence);

    float Damage = Spec.GetSetByCallerMagnitude("Damage", false, 0.f);
	if(Damage == 0.f)
	{
		GetCapturedAttributeMagnitude(SourceStrength, Spec, EvaluationParameters, Damage);
	}
	
    return Damage - Defence;

}