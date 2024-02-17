// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Paladyn.h"

#include "KrzyweKarty/Components/AreaEffectCharacterAbilityComponent.h"
#include "KrzyweKarty/Map/KKMap.h"


void AZakon_Paladyn::PerformAbility_Implementation(int32 Index)
{
	if(Index == 0)
	{
		UAreaEffectCharacterAbilityComponent* CharacterAbilityComponent = GetCharacterAbilityComponent<UAreaEffectCharacterAbilityComponent>(Index);
		TArray<AKKCharacter*> AffectedCharacters = GetMap()->GetCharactersByDirection(this, CharacterAbilityComponent->GetFinalAffectedTiles());

		for(AKKCharacter* Character : AffectedCharacters)
		{
			if(IsInTheSameTeam(Character))
			{
				Character->IncreaseHealth(4);
			}
			else
			{
				FAttackResultInfo AttackResultInfo;
				Character->ApplyDamageToSelf(10, AttackResultInfo);
			}
		}
	}
}

TArray<FDirection> AZakon_Paladyn::GetAffectedTiles_Implementation(int32 Index)
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
			{1, -1},
			{1, 1}
		};
		/*
			# #
			 O
		 */
	default:
		return {};
	}

	
}

