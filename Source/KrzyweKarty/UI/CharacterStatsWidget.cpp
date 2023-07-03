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
	
	if(Character != NewCharacter)
	{
		if(Character)
			Character->OnCharacterDeath.RemoveDynamic(this, &UCharacterStatsWidget::RemoveCharacter);
		
		Character = NewCharacter;
		CharacterNameText->SetText(Character->GetCharacterName());
		Character->OnCharacterDeath.AddUniqueDynamic(this, &UCharacterStatsWidget::RemoveCharacter);
		AddToViewport();
	}
	
	
}

void UCharacterStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

FText UCharacterStatsWidget::HealthText()
{
	if (!Character)
		return FText();

	float Value = (Character->GetDefaultHealth() != 0) ? (Character->GetHealth() / static_cast<float>(Character->GetDefaultHealth())) : 1;
	HealthImage->GetDynamicMaterial()->SetScalarParameterValue("Progress", Value);
	

	return FText::FormatOrdered(FormatText, Character->GetHealth());
}

FText UCharacterStatsWidget::ManaText()
{
	if (!Character)
		return FText();

	float Value = (Character->GetDefaultMana() != 0) ? (Character->GetMana() / static_cast<float>(Character->GetDefaultMana())) : 1;
    ManaImage->GetDynamicMaterial()->SetScalarParameterValue("Progress", Value);

	
	return FText::FormatOrdered(FormatText, Character->GetMana());
}

FText UCharacterStatsWidget::DefenceText()
{
	if (!Character)
		return FText();

	float Value = (Character->GetDefaultDefence() != 0) ? (Character->GetDefence() / static_cast<float>(Character->GetDefaultDefence())) : 1;
	DefenceImage->GetDynamicMaterial()->SetScalarParameterValue("Progress", Value);
	
	
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
	RemoveFromParent();
}
