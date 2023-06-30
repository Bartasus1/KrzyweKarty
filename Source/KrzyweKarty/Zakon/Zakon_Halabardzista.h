// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zakon.h"
#include "Zakon_Halabardzista.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Halabardzista : public AZakon
{
	GENERATED_BODY()
	
public:

	virtual bool ActiveAbility(AKKCharacter* TargetCharacter) override; 

	virtual  int32 GetStrengthAtDistance(int32 Distance) override;

	virtual TArray<FDirection> GetPossibleAttackTiles() override;
	
};
