// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zakon.h"
#include "Zakon_Kawalerzysta.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Kawalerzysta : public AZakon
{
	GENERATED_BODY()
public:
	
	virtual bool ActiveAbility(AKKCharacter* TargetCharacter) override;
	virtual bool ActiveAbility2(AKKCharacter* TargetCharacter) override;

	virtual TArray<FDirection> GetPossibleMoveTiles() override;
	
};
