// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zakon.h"
#include "Zakon_Rycerz.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API AZakon_Rycerz : public AZakon
{
	GENERATED_BODY()

	bool bAbleToBlock = false;
	uint8 AttacksCount = 0;
	
public:
	AZakon_Rycerz();
	
};
