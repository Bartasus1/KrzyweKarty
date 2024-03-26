// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Paladyn.h"
#include "KrzyweKarty/Cards/AbilityActor.h"
#include "KrzyweKarty/Interfaces/AbilityInterfaces/AreaAbilityInterface.h"
#include "KrzyweKarty/Map/KKMap.h"


void AZakon_Paladyn::PerformAbility_Implementation(uint8 Index)
{
	switch (Index)
	{
	case 0:
		{
			TArray<AKKCharacter*> AffectedCharacters = IAreaAbilityInterface::Execute_GetAffectedCharacters(AbilityActor);

			for(AKKCharacter* Character : AffectedCharacters)
			{
				if(IsInTheSameTeam(Character))
				{
					Character->IncreaseHealth(4);
				}
				else
				{
					FAttackResultInfo AttackResultInfo;
					Character->ApplyDamageToSelf(10, AttackResultInfo, this);
				}
			}
			
			break;
		}
	default:
		break;
	}
}

TArray<FDirection> AZakon_Paladyn::GetAffectedTiles_Implementation(uint8 Index)
{
	switch (Index)
	{
	case 0:
		return {
			{-1, 0},
			{1, -1},
			{1, 0},
			{1, 1},
			{2,0}
		};
		/*
			 #
			###
			 O
			 #
		*/
	case 1:
		return {
			{1, 0},
			{1, -1},
			{1, 1}
		};
		/*
		   @ # @
			 O
		 */
	default:
		return {};
	}

	
}

