// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Cards/KKCharacter.h"
#include "KrzyweKarty/Interfaces/AreaModifierInterface.h"
#include "Zakon_Paladyn.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Paladyn : public AKKCharacter, public IAreaModifierInterface
{
	GENERATED_BODY()
	
public:

	virtual void PerformAbility_Implementation(uint8 Index) override;

	virtual TArray<FDirection> GetAffectedTiles_Implementation(uint8 Index) override;
};
