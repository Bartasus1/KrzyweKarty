// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
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
	
	HealthImage->GetDynamicMaterial()->SetScalarParameterValue("Progress", Character->GetHealth() / static_cast<float>(Character->GetDefaultHealth()));
	

	return FText::FormatOrdered(FormatText, Character->GetHealth());
}

FText UCharacterStatsWidget::ManaText()
{
	if (!Character)
		return FText();
	
    ManaImage->GetDynamicMaterial()->SetScalarParameterValue("Progress", Character->GetMana() / static_cast<float>(Character->GetDefaultMana()));

	
	return FText::FormatOrdered(FormatText, Character->GetMana());
}

FText UCharacterStatsWidget::DefenceText()
{
	if (!Character)
		return FText();
	
	DefenceImage->GetDynamicMaterial()->SetScalarParameterValue("Progress", Character->GetDefence() / static_cast<float>(Character->GetDefaultDefence()));
	
	
	return FText::FormatOrdered(FormatText, Character->GetDefence());
}

FText UCharacterStatsWidget::StrengthText()
{
	if (!Character)
		return FText();

	return FText::FormatOrdered(FormatText, Character->GetStrength());
}

void UCharacterStatsWidget::RemoveCharacter()
{
	Character = nullptr;
	CharacterNameText->SetText(FText());
	RemoveFromParent();
}
