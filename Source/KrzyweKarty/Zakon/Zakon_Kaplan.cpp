// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Kaplan.h"

#include "KrzyweKarty/Cards/AbilityActor.h"
#include "KrzyweKarty/Interfaces/AbilityInterfaces/SelectorAbilityInterface.h"
#include "KrzyweKarty/Map/KKMap.h"

FAttackResultInfo AZakon_Kaplan::DefaultAttack(AKKCharacter* TargetCharacter)
{
	const FAttackResultInfo AttackResult = Super::DefaultAttack(TargetCharacter);
	
	if (AttackResult.AttackStatus == EAttackResult::AttackConfirmed)
	{
		IncreaseMana(3);
	}

	return AttackResult;
}

const TArray<AKKCharacter*> AZakon_Kaplan::FilterCharacters_Implementation(const TArray<AKKCharacter*>& InCharacters, uint8 Index)
{
	if(Index == 0)
	{
		InCharacters.FilterByPredicate([this](AKKCharacter* Character) -> bool
		{
			return IsInTheSameTeam(Character);
		});
	}

	return InCharacters;
}

void AZakon_Kaplan::PerformAbility_Implementation(uint8 Index)
{
	switch (Index)
	{
		case 0:
		{
			AKKCharacter* SelectedCharacter = ISelectorAbilityInterface::Execute_GetSelectedCharacter(AbilityActor);
			SelectedCharacter->IncreaseHealth(3);
			SelectedCharacter->IncreaseMana(4);
			break;
		}
		case 1:
		{
			AKKCharacter* SelectedCharacter = ISelectorAbilityInterface::Execute_GetSelectedCharacter(AbilityActor);

			FAttackResultInfo ResultInfo;
			SelectedCharacter->ApplyDamageToSelf(17, ResultInfo, this);
			break;
		}
		default:
			break;
	}
}
