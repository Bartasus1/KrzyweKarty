// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoundManager.generated.h"

class AKKCharacter;

UENUM(BlueprintType)
enum EMovementType
{
	EMP_SummonedCharacter	UMETA(DisplayName="Summoned Character"),
	EMP_MovedCharacter		UMETA(DisplayName="Moved Character"),
	EMP_AttackedCharacter	UMETA(DisplayName="Attacked Character")
};

USTRUCT()
struct FMovementInfo
{
	GENERATED_BODY()

	UPROPERTY()
	AKKCharacter* Character;

	UPROPERTY()
	TEnumAsByte<EMovementType> MovementType;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoundEndDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KRZYWEKARTY_API URoundManager : public UActorComponent
{
	GENERATED_BODY()

public:
	URoundManager();

	UPROPERTY(BlueprintAssignable)
	FRoundEndDelegate OnRoundEnd;

	void AddCharacterToList(AKKCharacter* Character, EMovementType MovementType);
	void ResetRound();

	bool CanUseCharacter(AKKCharacter* Character, EMovementType MovementType);


protected:
	TArray<FMovementInfo> CharactersUsedInRound;

	virtual void BeginPlay() override;
};
