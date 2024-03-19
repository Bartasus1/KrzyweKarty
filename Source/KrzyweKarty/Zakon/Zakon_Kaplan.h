// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Cards/KKCharacter.h"

#include "KrzyweKarty/Interfaces/AreaModifierInterface.h"

#include "Zakon_Kaplan.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Kaplan : public AKKCharacter, public IAreaModifierInterface
{
	GENERATED_BODY()
public:
	virtual FAttackResultInfo DefaultAttack(AKKCharacter* TargetCharacter) override;
	
	//virtual const TArray<AKKCharacter*>& FilterCharacters_Implementation(const TArray<AKKCharacter*>& InCharacters, uint8 Index) override;

	virtual void PerformAbility_Implementation(uint8 Index) override;
	virtual const TArray<AKKCharacter*> FilterCharacters_Implementation(const TArray<AKKCharacter*>& InCharacters, uint8 Index) override;
};
