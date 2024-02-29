// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KKGameStatics.generated.h"


enum class ETileState : uint8;
enum ERotationDirection : int;
struct FDirection;
class AKKTile;
class AKKCharacter;
class AKKMap;
class AKKPlayerController;

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API UKKGameStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static AKKPlayerController* GetKKPlayerController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static AKKMap* GetMap(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void AddActionLog(AKKCharacter* Character, AKKCharacter* TargetCharacter, FText Action);

	UFUNCTION(BlueprintCallable)
	static void RotateDirections(UPARAM(ref) TArray<FDirection>& Directions, ERotationDirection RotationDirection);
	

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static void HideTiles(const UObject* WorldContextObject);
};
