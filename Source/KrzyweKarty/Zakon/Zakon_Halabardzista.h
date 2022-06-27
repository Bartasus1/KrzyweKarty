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
	AZakon_Halabardzista();

	virtual void ActiveAbility() override; //Przedluzenie
	virtual void PassiveAbility() override; //Pika
};
