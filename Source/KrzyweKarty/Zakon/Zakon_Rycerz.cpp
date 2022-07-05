// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Rycerz.h"

AZakon_Rycerz::AZakon_Rycerz()
{
}

bool AZakon_Rycerz::ActiveAbility(AKKCharacter* TargetCharacter)
{
	if (GetMana() < 2)
		return false;

	IncreaseHealth(4);
	DecreaseMana(2);

	return true;
}

bool AZakon_Rycerz::CanBeAttacked(EAttackType AttackType)
{
	const bool CouldBeAttacked = Super::CanBeAttacked(AttackType);

	if (CouldBeAttacked)
	{
		AttacksCounter++;

		if (AttacksCounter >= 3)
		{
			AttacksCounter = 0;
			return false;
		}
	}

	return CouldBeAttacked;
}
