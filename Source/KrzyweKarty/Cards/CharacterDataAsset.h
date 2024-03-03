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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(GetOptions="GetFractions"))
	FName CharacterFraction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText CharacterFractionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> CharacterCardTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ShowOnlyInnerProperties))
	FCharacterStats CharacterStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(TitleProperty="{AbilityName}  ( {AbilityCost} PM )  --> {AbilityDescription}"), Category="Abilities")
	TArray<FAbilityDescription> ActiveAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(TitleProperty="{AbilityName} --> {AbilityDescription}"), Category="Abilities")
	TArray<FAbilityDescription> PassiveAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mesh and Animations")
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mesh and Animations")
	UAnimBlueprint* AnimBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh and Animations")
	TSoftObjectPtr<UAnimMontage> SummonMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh and Animations")
	TSoftObjectPtr<UAnimMontage> AttackMontage;
	
	
	UFUNCTION(BlueprintPure)
	TArray<FName> GetFractions();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
