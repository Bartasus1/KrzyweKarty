// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAbilityComponent.h"
#include "KrzyweKarty/Map/MapStructs.h"
#include "AreaEffectAbilityComponent.generated.h"


UCLASS(ClassGroup=(Custom))
class KRZYWEKARTY_API UAreaEffectAbilityComponent : public UCharacterAbilityComponent
{
	GENERATED_BODY()

public:
	UAreaEffectAbilityComponent();

	virtual void BeginAbility_Implementation() override;
	virtual void FinishAbility_Implementation(bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable)
	void RotateArea(ERotationDirection RotationDirection);

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECharacterSelectionPolicy> CharacterSelectionPolicy = CSP_AllCharacters;
	
	UPROPERTY()
	TArray<AKKTile*> AffectedTiles;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
