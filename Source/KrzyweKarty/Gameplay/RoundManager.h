// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoundManager.generated.h"

class AKKCharacter;
enum EMovementType;


UCLASS(Within="KKGameMode" ,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KRZYWEKARTY_API URoundManager : public UActorComponent
{
	friend class AKKGameMode;
	
	GENERATED_BODY()

public:
	URoundManager();


	
	void AddCharacterToList(AKKCharacter* Character, EMovementType MovementType);
	bool CanUseCharacter(AKKCharacter* Character, EMovementType MovementType);

protected:
	void ResetRound();

	int32 MaxMoves = 3;
	int32 MovesCounter = 0; //for cannons and other stuff - todo


	virtual void BeginPlay() override;
};