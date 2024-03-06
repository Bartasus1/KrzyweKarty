// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Cards/KKCharacter.h"
#include "Zakon_Kawalerzysta.generated.h"

struct FMapCell;
/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Kawalerzysta : public AKKCharacter
{
	GENERATED_BODY()
public:

	virtual bool CanUseAbility_Implementation(uint8 Index) override;

	virtual TArray<FDirection> GetPossibleMoveTiles() override;

private:

	TArray<FMapCell*> GetAbilityMapCells(uint8 Index);
	
};
