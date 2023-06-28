// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "FindSelectableAsync.generated.h"


class ISelectableInterface;
class UInputAction;
class UInputMappingContext;
class AKKPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFoundSelectable, TScriptInterface<ISelectableInterface>, Selectable);

UCLASS()
class KRZYWEKARTY_API UFindSelectableAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UFindSelectableAsync* FindSelectableAsync(const UObject* WorldContextObject, AKKPlayerController* PlayerController, UInputMappingContext* MappingContext, UInputAction* ClickAction);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FFoundSelectable SelectableFound;

private:

	UPROPERTY()
	AKKPlayerController* PlayerController;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere)
	UInputAction* ClickAction;

	UFUNCTION()
	void FindSelectable();
	
	
};
