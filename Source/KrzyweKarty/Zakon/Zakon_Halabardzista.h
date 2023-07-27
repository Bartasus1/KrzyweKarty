// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zakon.h"
#include "KrzyweKarty/Interfaces/AreaModifierInterface.h"
#include "Zakon_Halabardzista.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Halabardzista : public AZakon, public IAreaModifierInterface
{


private:
	GENERATED_BODY()

public:
	virtual int32 GetStrengthForAttack(AKKCharacter* TargetCharacter) override;
	

	TArray<FDirection> GetAffectedTiles_Implementation(int32 Index) override;	
};
