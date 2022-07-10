// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsWidget.h"
#include "Components/TextBlock.h"
#include "KrzyweKarty/Cards/KKCharacter.h"

void UCharacterStatsWidget::ShowStats(AKKCharacter* NewCharacter)
{
	Character = NewCharacter;

	CharacterNameText->SetText(Character->GetCharacterName());

	Character->OnCharacterDeath.AddUniqueDynamic(this, &UCharacterStatsWidget::RemoveCharacter);
}

void UCharacterStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

FText UCharacterStatsWidget::HealthText()
{
	if (!Character)
		return FText();

	return FText::FormatOrdered(FormatText, FText::FromString("Health"), Character->GetHealth(), Character->GetDefaultHealth());
}

FText UCharacterStatsWidget::ManaText()
{
	if (!Character)
		return FText();

	return FText::FormatOrdered(FormatText, FText::FromString("Mana"), Character->GetMana(), Character->GetDefaultMana());
}

FText UCharacterStatsWidget::DefenceText()
{
	if (!Character)
		return FText();

	return FText::FormatOrdered(FormatText, FText::FromString("Defence"), Character->GetDefence(), Character->GetDefaultDefence());
}

FText UCharacterStatsWidget::StrengthText()
{
	if (!Character)
		return FText();

	return FText::FormatOrdered(FormatText, FText::FromString("Strength"), Character->GetStrength(), Character->GetDefaultStrength());
}

void UCharacterStatsWidget::RemoveCharacter()
{
	Character = nullptr;
	CharacterNameText->SetText(FText());
	RemoveFromParent();
}
