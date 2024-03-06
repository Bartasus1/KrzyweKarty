// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Cards/KKCharacter.h"
#include "KrzyweKarty/Interfaces/AreaModifierInterface.h"
#include "Zakon_Lucznik.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Lucznik : public AKKCharacter, public IAreaModifierInterface
{
	GENERATED_BODY()

public:
	virtual FAttackResultInfo DefaultAttack(AKKCharacter* TargetCharacter) override;

	virtual void PerformAbility_Implementation(uint8 Index) override;
	
	virtual TArray<FDirection> GetAffectedTiles_Implementation(uint8 Index) override;

private:

	int8 AttacksCounter = 0;
};
