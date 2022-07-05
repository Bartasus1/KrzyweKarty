// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zakon.h"
#include "Zakon_Lucznik.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Lucznik : public AZakon
{
	GENERATED_BODY()
public:
	AZakon_Lucznik();

	virtual bool DefaultAttack(AKKCharacter* TargetCharacter) override;

	virtual bool ActiveAbility(AKKCharacter* TargetCharacter) override;

private:

	int8 AttacksCounter = 0;
};
