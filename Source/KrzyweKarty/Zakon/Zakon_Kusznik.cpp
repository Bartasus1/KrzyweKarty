// Fill out your copyright notice in the Description page of Project Settings.
#include "Zakon_Kusznik.h"
#include "KrzyweKarty/Cards/AbilityActor.h"
#include "KrzyweKarty/Interfaces/AbilityInterfaces/SelectorAbilityInterface.h"

FAttackResultInfo AZakon_Kusznik::DefaultAttack(AKKCharacter* TargetCharacter)
{
	FAttackResultInfo AttackResult = Super::DefaultAttack(TargetCharacter);

	if(AttackResult.AttackStatus == EAttackResult::AttackConfirmed)
	{
		IncreaseHealth(2);
		
		if(bSecondAbilityInUse)
		{
			bSecondAbilityInUse = false;
		}
	}
	
	return AttackResult;
}

int32 AZakon_Kusznik::DefineDamageAmount(AKKCharacter* TargetCharacter)
{
	if(bSecondAbilityInUse)
	{
		return GetStrength() + 2;
	}

	return GetStrength();
}

void AZakon_Kusznik::PerformAbility_Implementation(uint8 Index)
{
	switch (Index)
	{
	case 0:
		{
			AKKCharacter* SelectedCharacter = ISelectorAbilityInterface::Execute_GetSelectedCharacter(AbilityActor);

			FAttackResultInfo AttackResultInfo;
			SelectedCharacter->ApplyDamageToSelf(15, AttackResultInfo, this);
			SelectedCharacter->DecreaseDefence();
		}
	case 1:
		{
			IncreaseMana(2);
			bSecondAbilityInUse = true;
		}
	default:
		break;
	}
}

void AZakon_Kusznik::CommitAbilityCost_Implementation(uint8 Index)
{
	if(Index == 1)
	{
		DecreaseHealth(2);
	}
	else
	{
		Super::CommitAbilityCost_Implementation(Index);
	}
}

