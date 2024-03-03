// Fill out your copyright notice in the Description page of Project Settings.

#include "TileStatus/TileStatusFactory.h"
#include "KrzyweKarty/TileStatus/TileStatus.h"

UTileStatusFactory::UTileStatusFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UTileStatus::StaticClass();
}

UObject* UTileStatusFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UTileStatus>(InParent, InClass, InName, Flags);
}


/////////////////////////////////////////////////////////////


FText FTileStatusAssetTypeActions::GetName() const
{
	return FText::FromString("Tile Status");
}

UClass* FTileStatusAssetTypeActions::GetSupportedClass() const
{
	return UTileStatus::StaticClass();
}

FColor FTileStatusAssetTypeActions::GetTypeColor() const
{
	return FColor::Green;
}

uint32 FTileStatusAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Blueprint;
}
