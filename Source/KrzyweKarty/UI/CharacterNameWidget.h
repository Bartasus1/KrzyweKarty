// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterNameWidget.generated.h"

/**
 * 
 */

UCLASS()
class KRZYWEKARTY_API UCharacterNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterName;
};
