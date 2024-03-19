// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Halabardzista.h"
#include "KrzyweKarty/Cards/AbilityActor.h"
#include "KrzyweKarty/Interfaces/AbilityInterfaces/AreaAbilityInterface.h"
#include "KrzyweKarty/Map/KKMap.h"


void AZakon_Halabardzista::PerformAbility_Implementation(uint8 Index)
{
	TArray<AKKCharacter*> AffectedCharacters = IAreaAbilityInterface::Execute_GetAffectedCharacters(AbilityActor);

	for (AKKCharacter* Character : AffectedCharacters)
	{
		if(IsFirstCharacterInLine(Character))
		{
			int32 Damage = 0;
			switch (GetDistanceTo(Character))
			{
				case 1:
				case 2:
					Damage = 9;
					break;
				case 3:
					Damage = 7;
					break;
				default:
					break;
			}

			FAttackResultInfo AttackResultInfo;
			Character->ApplyDamageToSelf(Damage, AttackResultInfo);
		}
	}
}

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
