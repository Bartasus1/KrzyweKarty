// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "Materials/MaterialInstanceDynamic.h"

void UCharacterStatsWidget::ShowStats_Implementation(AKKCharacter* NewCharacter)
{
	if(NewCharacter == nullptr)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	if(Character) //if there was a character already, remove binding to it's death
	{
		Character->OnCharacterDeath.RemoveDynamic(this, &UCharacterStatsWidget::RemoveCharacter);
	}
	
	Character = NewCharacter; // assign new character
	CharacterNameText->SetText(Character->GetCharacterName());
	Character->OnCharacterDeath.AddUniqueDynamic(this, &UCharacterStatsWidget::RemoveCharacter);

	if(!IsInViewport())
	{
		AddToViewport();
	}
}

void UCharacterStatsWidget::ShowStatsPreview_Implementation(const FCharacterStats& PreviewStats)
{
	if(Character == nullptr)
	{
		return;
	}
	
	UpdateImageProperty("Progress Preview", PreviewStats.Health, Character->GetDefaultHealth(), HealthImage);
	UpdateImageProperty("Progress Preview", PreviewStats.Mana, Character->GetDefaultMana(), ManaImage);
	UpdateImageProperty("Progress Preview", PreviewStats.Defence, Character->GetDefaultDefence(), DefenceImage);
}

void UCharacterStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

FText UCharacterStatsWidget::HealthText()
{
	return GetTextForStat(&FCharacterStats::Health, HealthImage);
}

FText UCharacterStatsWidget::ManaText()
{
	return GetTextForStat(&FCharacterStats::Mana, ManaImage);
}

FText UCharacterStatsWidget::DefenceText()
{
	return GetTextForStat(&FCharacterStats::Defence, DefenceImage);
}

FText UCharacterStatsWidget::StrengthText()
{
	return GetTextForStat(&FCharacterStats::Strength, nullptr);
}

void UCharacterStatsWidget::UpdateImageProperty(FName PropertyName, float BaseValue, float MaxValue, UImage* StatImage) const
{
	if(StatImage)
	{
		const float Value = (MaxValue == 0) ? 0 : BaseValue / MaxValue;
		StatImage->GetDynamicMaterial()->SetScalarParameterValue(PropertyName, Value);
	}
}

void UCharacterStatsWidget::RemoveCharacter()
{
	Character = nullptr;
	SetVisibility(ESlateVisibility::Collapsed);
}

FText UCharacterStatsWidget::GetTextForStat(int32 FCharacterStats::* MemberField, UImage* StatImage) const
{
	if(Character == nullptr)
	{
		return FText();
	}

	const int32 BaseValue = Character->GetStat(MemberField);
	const int32 MaxValue = Character->GetDefaultStat(MemberField);
	
	UpdateImageProperty("Progress", BaseValue, MaxValue, StatImage);

	return FText::FormatOrdered(FormatText, BaseValue);
}
