// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Kismet/BlueprintAsyncActionBase.h"
#include "FindSelectableAsync.generated.h"


class ISelectableInterface;
class UInputAction;
class UInputMappingContext;
class AKKPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFoundSelectable, TScriptInterface<ISelectableInterface>, Selectable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFoundSelectableFail);

UCLASS()
class KRZYWEKARTY_API UFindSelectableAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UFindSelectableAsync* FindSelectableAsync(const UObject* WorldContextObject, AKKPlayerController* PlayerController, ECollisionChannel CollisionChannel, FGameplayTag EnterGameplayTag, FGameplayTag ExitGameplayTag);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FFoundSelectable SelectableFound;
	
	UPROPERTY(BlueprintAssignable)
	FFoundSelectableFail SelectableNotFound;

private:

	UPROPERTY()
	AKKPlayerController* PlayerController;

	UPROPERTY()
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY()
	FGameplayTag EnterGameplayTag;

	UPROPERTY()
	FGameplayTag ExitGameplayTag;

	UFUNCTION()
	void FindSelectable();

	UFUNCTION()
	void ReportFail();
};
