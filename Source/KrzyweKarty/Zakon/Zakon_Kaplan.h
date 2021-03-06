// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zakon.h"
#include "Zakon_Kaplan.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Kaplan : public AZakon
{
	GENERATED_BODY()
public:
	AZakon_Kaplan();

	virtual bool DefaultAttack(AKKCharacter* TargetCharacter) override;
	
	virtual bool ActiveAbility(AKKCharacter* TargetCharacter) override;
	virtual bool ActiveAbility2(AKKCharacter* TargetCharacter) override;

};
