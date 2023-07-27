#include "AbilityExecutor.h"
#include "KrzyweKarty/Gameplay/KKGameState.h"
#include "KrzyweKarty/Map/KKMap.h"


AAbilityExecutor::AAbilityExecutor()
{

	PrimaryActorTick.bCanEverTick = false;
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

AKKMap* AAbilityExecutor::GetMap()
{
	return GetGameState()->Map;
}

AKKGameState* AAbilityExecutor::GetGameState()
{
	return GetWorld()->GetGameState<AKKGameState>();
}

ERotationDirection AAbilityExecutor::GetRotationFromInt(int32 Number)
{
	switch (Number)
	{
	case 1:
		return ERD_Forward;
	case 2:
		return ERD_Right;
	case 3:
		return ERD_Backward;
	case 4:
		return ERD_Left;
	
	default:
		return ERD_Forward;
	}
}


void AAbilityExecutor::SR_ExecuteAbility_Implementation()
{
	ExecuteAbility();
}

void AAbilityExecutor::SR_EndAbility_Implementation()
{
	EndAbility();
}