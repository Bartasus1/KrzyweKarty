// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundManager.h"
#include "KrzyweKarty/Cards/KKCharacter.h"


// Sets default values for this component's properties
URoundManager::URoundManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void URoundManager::AddCharacterToList(AKKCharacter* Character, EMovementType MovementType)
{

}

void URoundManager::ResetRound()
{

}

bool URoundManager::CanUseCharacter(AKKCharacter* Character, EMovementType MovementType)
{
	return false;
}


// Called when the game starts
void URoundManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
}
