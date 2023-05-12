// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoundManager.generated.h"

class AKKCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoundEndDelegate);


UENUM(BlueprintType)
enum EMovementType
{
	EMP_SummonedCharacter	UMETA(DisplayName="Summoned Character"),
	EMP_MovedCharacter		UMETA(DisplayName="Moved Character"),
	EMP_AttackCharacter		UMETA(DisplayName="Attacked Character")
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

UCLASS(Within="KKGameMode" ,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KRZYWEKARTY_API URoundManager : public UActorComponent
{
	friend class AKKGameMode;
	
	GENERATED_BODY()

public:
	URoundManager();

	UPROPERTY(BlueprintAssignable)
	FRoundEndDelegate OnRoundEnd;
	
	void AddCharacterToList(AKKCharacter* Character, EMovementType MovementType);
	bool CanUseCharacter(AKKCharacter* Character, EMovementType MovementType);

protected:
	void ResetRound();

	int32 MaxMoves = 3;
	int32 MovesCounter = 0; //for cannons and other stuff - todo
	TArray<FMovementInfo> CharactersUsedInRound;

	virtual void BeginPlay() override;
};