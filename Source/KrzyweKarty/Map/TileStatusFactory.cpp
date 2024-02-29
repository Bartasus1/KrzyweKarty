// Fill out your copyright notice in the Description page of Project Settings.


#include "TileStatusFactory.h"
#include "TileStatus.h"

UTileStatusFactory::UTileStatusFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	bEditorImport = false;
	SupportedClass = UTileStatus::StaticClass();
}

bool UTileStatusFactory::ShouldShowInNewMenu() const
{
	return true;
}

FText UTileStatusFactory::GetDisplayName() const
{
	return FText::FromString("TileStatus");
}

UObject* UTileStatusFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UTileStatus>(InParent, InClass, InName, Flags, Context);
}
