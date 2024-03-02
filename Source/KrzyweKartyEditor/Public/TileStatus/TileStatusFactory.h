// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "Factories/Factory.h"
#include "TileStatusFactory.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTYEDITOR_API UTileStatusFactory : public UFactory
{
	GENERATED_BODY()
public:
	UTileStatusFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

/////////////////////////////////////////////////////////////////////

class  FTileStatusAssetTypeActions : public FAssetTypeActions_Base
{

public:
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
};
