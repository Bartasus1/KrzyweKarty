// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHelpersSettings.h"
#include "Cards/CharacterDataAsset.h"

UCharacterHelpersSettings::UCharacterHelpersSettings()
{
	UCharacterDataAsset* CharacterDataDO = UCharacterDataAsset::StaticClass()->GetDefaultObject<UCharacterDataAsset>();

	for (const FName& FractionName : CharacterDataDO->GetFractions())
	{
		FractionTextMap.Add(FractionName);
	}
}
