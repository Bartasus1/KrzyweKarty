// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Rycerz.h"

// bool AZakon_Rycerz::ActiveAbility()
// {
// 	if (GetMana() < GetFirstAbilityManaCost())
// 		return false;
//
// 	IncreaseHealth(4);
// 	DecreaseManaForFirstAbility();
//
// 	return true;
// }

void AZakon_Rycerz::ApplyDamageToSelf(int32 DamageAmount, FAttackResultInfo& AttackResultInfo)
{
	ReceivedAttacksCounter++;

	if(ReceivedAttacksCounter >= 3)
	{
		ReceivedAttacksCounter = 0;
		
		AttackResultInfo.AttackResultEnum = EAttackResult::AttackBlocked;
		AttackResultInfo.AttackResultText = FText::FromString("Knight blocked the attack due to his Passive Ability");
	}
	else
	{
		Super::ApplyDamageToSelf(DamageAmount, AttackResultInfo);
	}
}
