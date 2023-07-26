// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Kaplan.h"
#include "KrzyweKarty/Map/KKMap.h"

bool AZakon_Kaplan::DefaultAttack(AKKCharacter* TargetCharacter)
{
	if (Super::DefaultAttack(TargetCharacter))
	{
		IncreaseMana(3);
		return true;
	}

	return false;
}

// bool AZakon_Kaplan::ActiveAbility()
// {
// 	if (GetMana() < GetFirstAbilityManaCost())
// 		return false;
//
// 	
//
// 	DecreaseManaForFirstAbility();
// 	return true;
// }
//
// bool AZakon_Kaplan::ActiveAbility2()
// {
// 	if (GetMana() < GetSecondAbilityManaCost())
// 		return false;
//
// 	// if (DefaultAttackConditions(ReverseState, EAT_ActiveAbility))
// 	// {
// 	// 	DealDamage(ReverseState, 17);
// 	// 	
// 	// 	DecreaseManaForSecondAbility();
// 	// 	return true;
// 	// }
//
// 	return false;
// }

