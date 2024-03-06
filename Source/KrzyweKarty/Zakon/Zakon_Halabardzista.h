// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Cards/KKCharacter.h"
#include "KrzyweKarty/Interfaces/AreaModifierInterface.h"
#include "Zakon_Halabardzista.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Halabardzista : public AKKCharacter, public IAreaModifierInterface
{


private:
	GENERATED_BODY()

public:
	virtual int32 DefineDamageAmount(AKKCharacter* TargetCharacter) override;
	

	virtual TArray<FDirection> GetAffectedTiles_Implementation(uint8 Index) override;	
};
