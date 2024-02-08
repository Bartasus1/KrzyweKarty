// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Rycerz.h"

#include "KrzyweKarty/Gameplay/KKGameState.h"

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
		
		GetGameState()->Multicast_AddActionLog(FText::FromString("Rycerz zablokowal atak dzieki swojej umiejetnosci pasywnej: Fechtunek")); //todo: move it to some kind of DataTable
	}
	else
	{
		Super::ApplyDamageToSelf(DamageAmount, AttackResultInfo);
	}
}

void AZakon_Rycerz::PerformAbility_Implementation(int32 Index)
{
	IncreaseHealth(4);
}
