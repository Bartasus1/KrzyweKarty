// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing="OnRep_Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing="OnRep_Defence")
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Defence);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing="OnRep_Strength")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Strength);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing="OnRep_Mana")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Mana);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_Defence(const FGameplayAttributeData& OldDefence);

	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
