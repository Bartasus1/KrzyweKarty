// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zakon.h"
#include "Zakon_Rycerz.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Rycerz : public AZakon
{
	GENERATED_BODY()

public:
	virtual bool ActiveAbility(AKKCharacter* TargetCharacter) override;

protected:
	virtual bool CanBeAttacked(EAttackType AttackType) override;

private:
	int8 AttacksCounter = 0;
};
