// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zakon.h"
#include "KrzyweKarty/Interfaces/AreaModifierInterface.h"
#include "Zakon_Paladyn.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Paladyn : public AZakon, public IAreaModifierInterface
{
	GENERATED_BODY()
public:
	virtual bool ActiveAbility(AKKCharacter* TargetCharacter) override;
	virtual bool ActiveAbility2(AKKCharacter* TargetCharacter) override;

	virtual bool CanBeAttacked(EAttackType AttackType) override;
protected:
	virtual TArray<FDirection> GetAffectedTiles(int32 Index) override;
};
