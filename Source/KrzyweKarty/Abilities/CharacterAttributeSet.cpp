// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeSet.h"

#include "Net/UnrealNetwork.h"

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, Health.GetBaseValue());
	}
	if(Attribute == GetDefenceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, Defence.GetBaseValue());
	}
	if(Attribute == GetStrengthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, Strength.GetBaseValue());
	}
	if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, Mana.GetBaseValue());
	}
}

void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
}

void UCharacterAttributeSet::OnRep_Defence(const FGameplayAttributeData& OldDefence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Defence, OldDefence);
}

void UCharacterAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Strength, OldStrength);
}

void UCharacterAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Mana, OldMana);
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Defence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}
