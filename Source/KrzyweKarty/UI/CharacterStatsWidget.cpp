// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsWidget.h"
#include "Components/TextBlock.h"
#include "KrzyweKarty/Gameplay/KKCharacter.h"

void UCharacterStatsWidget::ShowStats(AKKCharacter* Character)
{
	if (!CharacterNameText && !CharacterHealthText && !CharacterManaText && !CharacterDefenceText && !CharacterDefenceText)
		return;
	
	
	CharacterNameText->SetText(Character->CharacterName);
	
	CharacterHealthText->SetText(FText::AsNumber(Character->CharacterStats.Health));
	CharacterManaText->SetText(FText::AsNumber(Character->CharacterStats.Mana));
	CharacterDefenceText->SetText(FText::AsNumber(Character->CharacterStats.Defence));
	CharacterStrengthText->SetText(FText::AsNumber(Character->CharacterStats.Strength));
}

void UCharacterStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
