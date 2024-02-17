// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Cards/KKCharacter.h"
#include "Zakon_Kaplan.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Kaplan : public AKKCharacter
{
	GENERATED_BODY()
public:
	virtual FAttackResultInfo DefaultAttack(AKKCharacter* TargetCharacter) override;
	

};
