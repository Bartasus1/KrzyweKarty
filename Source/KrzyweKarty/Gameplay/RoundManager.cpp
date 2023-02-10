// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundManager.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
URoundManager::URoundManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	CharactersUsedInRound.Reserve(3);
	// ...
}

void URoundManager::AddCharacterToList(AKKCharacter* Character, EMovementType MovementType)
{
	CharactersUsedInRound.Add({Character, MovementType});

	if (CharactersUsedInRound.Num() >= MaxMoves)
	{
		ResetRound();
	}
}

void URoundManager::ResetRound()
{
	CharactersUsedInRound.Reset(3);
	OnRoundEnd.Broadcast();
}

bool URoundManager::CanUseCharacter(AKKCharacter* Character, EMovementType MovementType)
{
	for(auto& Move : CharactersUsedInRound)
	{
		if(Move.Character == Character)
		{
			if(Move.MovementType >= MovementType)
			{
				return false;
			}
		}
	}

	return true;
}


// Called when the game starts
void URoundManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
}
