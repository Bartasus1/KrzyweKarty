// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDataAsset.h"

TArray<FName> UCharacterDataAsset::GetFractions()
{
	return {
		"Order",
		"Dwarves",
		"Elves",
		"Orcs",
		"Demons",
		"Undead",
		"Elements",
		"Entho",
		"Forgerin",
		"Robo"
	};
}
