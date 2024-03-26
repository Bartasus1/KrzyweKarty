// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Rycerz.h"
#include "KrzyweKarty/Gameplay/KKGameState.h"

void AZakon_Rycerz::ApplyDamageToSelf(int32 DamageAmount, FAttackResultInfo& AttackResultInfo, AKKCharacter* DamageCauser)
{
	ReceivedAttacksCounter++;

	if(ReceivedAttacksCounter >= 3)
	{
		ReceivedAttacksCounter = 0;
		
		GetGameState()->Multicast_AddActionLog(FText::FromString("Rycerz zablokowal atak dzieki swojej umiejetnosci pasywnej: Fechtunek")); //todo: move it to some kind of DataTable; add visuals
	}
	else
	{
		Super::ApplyDamageToSelf(DamageAmount, AttackResultInfo, DamageCauser);
	}
}

void AZakon_Rycerz::PerformAbility_Implementation(uint8 Index)
{
	IncreaseHealth(4);
}
