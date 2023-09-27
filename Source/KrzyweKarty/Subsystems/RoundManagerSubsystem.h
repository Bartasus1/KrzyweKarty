// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RoundManagerSubsystem.generated.h"

class AKKPlayerController;
class AKKCharacter;
/**
 * 
 */
UCLASS()
class KRZYWEKARTY_API URoundManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintAuthorityOnly)
	void RegisterCharacterInSystem(AKKCharacter* Character);
	
	UFUNCTION(BlueprintAuthorityOnly)
	void RegisterPlayerInSystem(AKKPlayerController* PlayerController);

	UFUNCTION(BlueprintAuthorityOnly)
	void ChangeTurn();

private:

	UPROPERTY(VisibleAnywhere)
	TArray<AKKPlayerController*> RegisteredPlayers;

	UPROPERTY(VisibleAnywhere)
	TSet<AKKCharacter*> RegisteredCharacters;

	const int32 MovesPerTurn = 3;
	int32 CountedMoves = 0;

	bool bFirstPlayerTurn = true;
};
