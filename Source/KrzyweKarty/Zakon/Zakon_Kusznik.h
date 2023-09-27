// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zakon.h"
#include "Zakon_Kusznik.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Kusznik : public AZakon
{
	GENERATED_BODY()
public:
	virtual FAttackResultInfo DefaultAttack(AKKCharacter* TargetCharacter) override;

	virtual int32 DefineDamageAmount(AKKCharacter* TargetCharacter) override;

private:
	bool SecondAbilityInUse = false;
};
