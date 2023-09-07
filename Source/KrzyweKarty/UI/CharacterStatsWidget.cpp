// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "KrzyweKarty/Cards/KKCharacter.h"

void UCharacterStatsWidget::ShowStats_Implementation(AKKCharacter* NewCharacter)
{
	if(NewCharacter == nullptr)
	{
		RemoveFromParent();
		return;
	}
	
	if(Character)
	{
		Character->OnCharacterDeath.RemoveDynamic(this, &UCharacterStatsWidget::RemoveCharacter);
	}
	
	Character = NewCharacter;
	CharacterNameText->SetText(Character->GetCharacterName());
	Character->OnCharacterDeath.AddUniqueDynamic(this, &UCharacterStatsWidget::RemoveCharacter);

	if(!IsInViewport())
	{
		AddToViewport();
	}
}

void UCharacterStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

FText UCharacterStatsWidget::HealthText()
{
	return GetTextForStat(Character->GetHealth(), Character->GetDefaultHealth(), HealthImage);
}

FText UCharacterStatsWidget::ManaText()
{
	return GetTextForStat(Character->GetMana(), Character->GetDefaultMana(), ManaImage);
}

FText UCharacterStatsWidget::DefenceText()
{
	return GetTextForStat(Character->GetDefence(), Character->GetDefaultDefence(), DefenceImage);
}

FText UCharacterStatsWidget::StrengthText()
{
	return GetTextForStat(Character->GetStrength(), Character->GetDefaultStrength(), nullptr);
}

void UCharacterStatsWidget::RemoveCharacter()
{
	Character = nullptr;
	RemoveFromParent();
}

FText UCharacterStatsWidget::GetTextForStat(float BaseValue, float MaxValue, UImage* StatImage) const
{
	if (!Character)
		return FText();

	if(StatImage)
	{
		float Value = (Character->GetDefaultDefence() != 0) ? (BaseValue / MaxValue) : 1;
        StatImage->GetDynamicMaterial()->SetScalarParameterValue("Progress", Value);
	}
	
	return FText::FormatOrdered(FormatText, BaseValue);
}

