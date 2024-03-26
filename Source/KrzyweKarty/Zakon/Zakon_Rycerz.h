// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Cards/KKCharacter.h"
#include "Zakon_Rycerz.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Rycerz : public AKKCharacter
{
	GENERATED_BODY()

protected:
	virtual void ApplyDamageToSelf(int32 DamageAmount, FAttackResultInfo& AttackResultInfo, AKKCharacter* DamageCauser) override;

	virtual void PerformAbility_Implementation(uint8 Index) override;

private:
	uint8 ReceivedAttacksCounter = 0;
};
