// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Kusznik.h"
#include "KrzyweKarty/Map/KKMap.h"

FAttackResultInfo AZakon_Kusznik::DefaultAttack(AKKCharacter* TargetCharacter)
{
	FAttackResultInfo AttackResult = Super::DefaultAttack(TargetCharacter);

	if(AttackResult.AttackResultEnum == EAttackResult::AttackConfirmed)
	{
		IncreaseHealth(2);
	}
	
	return AttackResult;
}

int32 AZakon_Kusznik::DefineDamageAmount(AKKCharacter* TargetCharacter)
{
	if(SecondAbilityInUse)
	{
		SecondAbilityInUse = false;
		return GetStrength() + 2;
	}

	return GetStrength();
}

// bool AZakon_Kusznik::ActiveAbility()
// {
// 	if(GetMana() < GetFirstAbilityManaCost())
// 		return false;
//
// 	// if(!DefaultAttackConditions(ReverseState, EAT_ActiveAbility))
// 	// 	return false;
// 	//
// 	// DealDamage(ReverseState, 15);
// 	// ReverseState->DecreaseDefence();
//
// 	DecreaseManaForFirstAbility();
// 	return true;
// }
//
// bool AZakon_Kusznik::ActiveAbility2()
// {
// 	if(GetHealth() < GetSecondAbilityManaCost())
// 		return false;
//
// 	IncreaseMana(2);
// 	SecondAbilityInUse = true;
// 	DecreaseHealth(GetSecondAbilityManaCost());
// 	return true;
// }

