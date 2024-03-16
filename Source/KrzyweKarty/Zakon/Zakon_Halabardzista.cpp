// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Halabardzista.h"
#include "KrzyweKarty/Map/KKMap.h"


int32 AZakon_Halabardzista::DefineDamageAmount(AKKCharacter* TargetCharacter)
{
	switch (GetDistanceTo(TargetCharacter))
	{
		case 1: return GetStrength();
		case 2: return  6;
		default: return 0;
	}
}

TArray<FDirection> AZakon_Halabardzista::GetAffectedTiles_Implementation(uint8 Index)
{
	return {
		{1, 0},
		{2, 0},
		{3, 0}
	};
}
