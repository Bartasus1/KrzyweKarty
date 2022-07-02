// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsWidget.h"
#include "Components/TextBlock.h"
#include "KrzyweKarty/Gameplay/KKCharacter.h"

void UCharacterStatsWidget::ShowStats(AKKCharacter* Character)
{
	if (!CharacterNameText && !CharacterHealthText && !CharacterManaText && !CharacterDefenceText && !CharacterDefenceText)
		return;
	
	CharacterNameText->SetText(Character->CharacterName);

	const FText FormatText = FText::FromString("{0} / {1}");
	
	CharacterHealthText->SetText(FText::Format(FormatText, Character->GetHealth(), Character->GetDefaultHealth()));
	CharacterManaText->SetText(FText::Format(FormatText, Character->GetMana(), Character->GetDefaultMana()));
	CharacterDefenceText->SetText(FText::Format(FormatText, Character->GetDefence(), Character->GetDefaultDefence()));
	CharacterStrengthText->SetText(FText::Format(FormatText, Character->GetStrength(), Character->GetDefaultStrength()));
}

void UCharacterStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}
