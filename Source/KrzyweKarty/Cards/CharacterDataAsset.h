// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStructs.h"
#include "Engine/DataAsset.h"
#include "CharacterDataAsset.generated.h"


UCLASS(BlueprintType)
class KRZYWEKARTY_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(GetOptions="GetFractions"))
	FName CharacterFraction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CharacterCardTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterStats CharacterStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityDescription> ActiveAbilitiesDescriptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityDescription> PassiveAbilitiesDescriptions;
	
	
	UFUNCTION(BlueprintPure)
	TArray<FName> GetFractions();
};
