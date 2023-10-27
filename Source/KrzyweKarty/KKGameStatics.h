﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Map/KKMap.h"
#include "Map/KKTile.h"
#include "KKGameStatics.generated.h"

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API UKKGameStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static AKKMap* GetMap(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void AddActionLog(AKKCharacter* Character, AKKCharacter* TargetCharacter, FText Action);

	UFUNCTION(BlueprintCallable)
	static TArray<FDirection> RotateDirections(TArray<FDirection> Directions, ERotationDirection RotationDirection);

	UFUNCTION(BlueprintCallable)
	static void ShowTiles(TArray<AKKTile*> Tiles, ETileColor Color);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static void HideTiles(const UObject* WorldContextObject);
};
