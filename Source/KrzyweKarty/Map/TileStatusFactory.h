// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "TileStatusFactory.generated.h"

/**
 * 
 */
UCLASS()
class UTileStatusFactory : public UFactory
{
	GENERATED_BODY()

public:

	UTileStatusFactory();
	
	virtual bool ShouldShowInNewMenu() const override;
	virtual FText GetDisplayName() const override;
	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
