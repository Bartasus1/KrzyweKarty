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
	AZakon_Kawalerzysta();

	//virtual void ActiveAbility() override; //Taranowanie
	//virtual void ActiveAbility2() override; //Wykop
	//virtual void PassiveAbility() override; // Jazda Konna
};
