// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zakon.h"
#include "KrzyweKarty/Interfaces/AreaModifierInterface.h"
#include "Zakon_Lucznik.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Lucznik : public AZakon, public IAreaModifierInterface
{
	GENERATED_BODY()

public:
	virtual FAttackResultInfo DefaultAttack(AKKCharacter* TargetCharacter) override;

	virtual void PerformAbility_Implementation(int32 Index) override;
	
	virtual TArray<FDirection> GetAffectedTiles_Implementation(int32 Index) override;

private:

	int8 AttacksCounter = 0;
};
