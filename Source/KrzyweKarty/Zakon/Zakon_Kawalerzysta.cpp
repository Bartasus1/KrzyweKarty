// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Kawalerzysta.h"
#include "KrzyweKarty/Map/KKMap.h"

bool AZakon_Kawalerzysta::ActiveAbility(AKKCharacter* TargetCharacter)
{
	return Super::ActiveAbility(TargetCharacter);
}

bool AZakon_Kawalerzysta::ActiveAbility2(AKKCharacter* TargetCharacter)
{
	return Super::ActiveAbility2(TargetCharacter);
}

TArray<FDirection> AZakon_Kawalerzysta::GetPossibleMoveTiles()
{
	return{
		{-1, 0},
		{1, 0},
		{2, 0},
		{0 , -1},
		{0, 1}
	};
}

