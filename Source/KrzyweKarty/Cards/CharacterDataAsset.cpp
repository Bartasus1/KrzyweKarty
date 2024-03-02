// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDataAsset.h"
#include "KrzyweKarty/CharacterHelpersSettings.h"

TArray<FName> UCharacterDataAsset::GetFractions()
{
	return {
		"Order",
		"Dwarves",
		"Elves",
		"Orcs",
		"Demons",
		"Undead",
		"Elements",
		"Entho",
		"Forgerin",
		"Robo"
	};
}

void UCharacterDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	const UCharacterHelpersSettings* CharacterHelpersSettings = UCharacterHelpersSettings::Get();
	CharacterFractionText = CharacterHelpersSettings->FractionTextMap[CharacterFraction];
	
}
