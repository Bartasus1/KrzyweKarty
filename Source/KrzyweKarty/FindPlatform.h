// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "FindPlatform.generated.h"


class UInputAction;
class UInputMappingContext;
class AKKTile;
class AKKPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFoundPlatform, AKKTile*, Tile);

UCLASS()
class KRZYWEKARTY_API UFindPlatform : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UFindPlatform* FindPlatform(const UObject* WorldContextObject, AKKPlayerController* PlayerController, UInputMappingContext* MappingContext, UInputAction* ClickAction);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FFoundPlatform PlatformFound;

private:

	UPROPERTY()
	AKKPlayerController* PlayerController;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere)
	UInputAction* ClickAction;

	UFUNCTION()
	void LookForPlatform();
	
	
};
