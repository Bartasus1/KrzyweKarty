// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Engine/DataAsset.h"
#include "PlayerInputDataAsset.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API UPlayerInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

	friend class AKKPlayerController;
	
public:

	/* Mapping Contexts */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mapping Contexts")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mapping Contexts")
	UInputMappingContext* SelectionMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mapping Contexts")
	UInputMappingContext* AbilityMappingContext;

	/* Gameplay Actions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actions")
	UInputAction* Selection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actions")
	UInputAction* SelectionCancel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actions")
	UInputAction* FindSelectable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actions")
	UInputAction* FindSelectableCancel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actions")
	UInputAction* AbilityConfirm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actions")
	UInputAction* AbilityCancel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actions")
	UInputAction* RotateDirections;

private:
	static UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem(AKKPlayerController* PlayerController);

	static void AddMappingContext(AKKPlayerController* PlayerController, UInputMappingContext* MappingContext, int32 Priority);
	static void RemoveMappingContext(AKKPlayerController* PlayerController, UInputMappingContext* MappingContext);

	void RemoveMappingContext(AKKPlayerController* PlayerController, UInputMappingContext* UPlayerInputDataAsset::*MemberMappingContext);
};
