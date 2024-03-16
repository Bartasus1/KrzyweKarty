// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "Materials/MaterialInstanceDynamic.h"

void UCharacterStatsWidget::ShowStats_Implementation(AKKCharacter* NewCharacter)
{
	SetVisibility(NewCharacter ? ESlateVisibility::Visible: ESlateVisibility::Collapsed);
	
	if(NewCharacter == nullptr)
	{
		return;
	}
	
	if(Character) //if there was a character already, remove binding to it's death
	{
		Character->OnCharacterDeath.RemoveDynamic(this, &UCharacterStatsWidget::RemoveCharacter);
		Character->OnCharacterStatsChanged.RemoveDynamic(this, &UCharacterStatsWidget::UpdateStatistics);
	}
	
	Character = NewCharacter; // assign new character
	CharacterNameText->SetText(Character->GetCharacterName());
	CharacterImage->SetBrushFromSoftTexture(Character->CharacterDataAsset->CharacterCardTexture);
	UpdateStatistics();
	
	Character->OnCharacterDeath.AddUniqueDynamic(this, &UCharacterStatsWidget::RemoveCharacter);
	Character->OnCharacterStatsChanged.AddUniqueDynamic(this, &UCharacterStatsWidget::UpdateStatistics);
}

void UCharacterStatsWidget::ShowStatsPreview_Implementation(const FCharacterStats& PreviewStats)
{
	if(Character == nullptr)
	{
		return;
	}

	UpdateImageProperty("Progress", PreviewStats.Health, Character->GetDefaultHealth(), HealthImage);
	UpdateImageProperty("Progress", PreviewStats.Mana, Character->GetDefaultMana(), ManaImage);
	UpdateImageProperty("Progress", PreviewStats.Defence, Character->GetDefaultDefence(), DefenceImage);
	
	UpdateImageProperty("Progress Preview", Character->GetHealth(), Character->GetDefaultHealth(), HealthImage);
	UpdateImageProperty("Progress Preview", Character->GetMana(), Character->GetDefaultMana(), ManaImage);
	UpdateImageProperty("Progress Preview", Character->GetDefence(), Character->GetDefaultDefence(), DefenceImage);
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

void UCharacterStatsWidget::UpdateStatistics()
{
	CharacterHealthText->SetText(HealthText());
	CharacterDefenceText->SetText(DefenceText());
	CharacterManaText->SetText(ManaText());
	CharacterStrengthText->SetText(StrengthText());

	ShowStatsPreview(Character->GetCharacterStats());
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

	const int32 BaseValue = Character->GetCharacterStatistic(MemberField);
	const int32 MaxValue = Character->GetCharacterDefaultStatistic(MemberField);
	
	UpdateImageProperty("Progress", BaseValue, MaxValue, StatImage);

	return FText::FormatOrdered(FormatText, BaseValue);
}
