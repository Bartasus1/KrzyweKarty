// Fill out your copyright notice in the Description page of Project Settings.



#include "KKCharacter.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"
#include "KrzyweKarty/Map/KKMap.h"
#include "KrzyweKarty/Gameplay/KKGameMode.h"
#include "KrzyweKarty/Gameplay/KKGameState.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"
#include "KrzyweKarty/Interfaces/BaseInterface.h"
#include "KrzyweKarty/Map/KKTile.h"
#include "KrzyweKarty/CharacterHelpersSettings.h"
#include "Core/Public/Containers/Array.h"

// Sets default values
AKKCharacter::AKKCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	AActor::SetReplicateMovement(true);

	Platform = CreateDefaultSubobject<UStaticMeshComponent>("Platform");
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>("CharacterMesh");
	TextRenderName = CreateDefaultSubobject<UTextRenderComponent>("TextRenderName");

	SetRootComponent(Platform);
	CharacterMesh->SetupAttachment(Platform);
	TextRenderName->SetupAttachment(Platform);

	CharacterMesh->SetRelativeRotation(FRotator(0, -90, 0));
	CharacterMesh->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));
	CharacterMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CharacterMesh->SetCollisionResponseToChannel(SelectableTraceChannel, ECR_Block);
	CharacterMesh->SetCastShadow(false);
	
	TextRenderName->SetRelativeLocation(FVector(0, 0, 110));
	TextRenderName->SetTextRenderColor(FColor::Red);
	TextRenderName->SetHorizontalAlignment(EHTA_Center);
	TextRenderName->SetWorldSize(18.f);
	
	Platform->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	Platform->SetCollisionResponseToChannel(SelectableTraceChannel, ECR_Block);
}

void AKKCharacter::CharacterDied_Implementation()
{
	OnCharacterDeath.Broadcast();
}

int32 AKKCharacter::GetTilePositionID()
{
	return OwnedTileID;
}

TArray<int32> AKKCharacter::GetPossibleSpawnTiles()
{
	return { 0, 1, 2 ,3};
}

TArray<FDirection> AKKCharacter::GetPossibleMoveTiles()
{
	return {
		{-1, 0},
		{1, 0},
		{0 , -1},
		{0, 1}
	};
}

TArray<FDirection> AKKCharacter::GetPossibleAttackTiles()
{
	TArray<FDirection> DefaultAttackTiles;
	const int32 AttackRange = CharacterStats.MaxAttackRange;
	
	for(int32 i = -AttackRange; i <= AttackRange; i++)
	{
		if(i == 0)
			continue;
		
		DefaultAttackTiles.Add({i, 0});
		DefaultAttackTiles.Add({0, i});
	}
	
	return DefaultAttackTiles;
}

TArray<AKKTile*> AKKCharacter::GetMoveTiles()
{
	return GetMap()->GetTilesByDirection(this, GetPossibleMoveTiles(), TSP_NoCharacters);
}

TArray<AKKTile*> AKKCharacter::GetAttackTiles()
{
	return GetMap()->CanAttackBase(this, GetMap()->GetTilesByDirection(this, GetPossibleAttackTiles(), TSP_EnemyCharactersOnly));
}

void AKKCharacter::HighlightDefaultAttackTiles()
{
	for(AKKTile* Tile: GetAttackTiles())
	{
		Tile->SetTileColor(Red);
	}
}

void AKKCharacter::HighlightMoveTiles()
{
	for(AKKTile* Tile: GetMoveTiles())
	{
		Tile->SetTileColor(Yellow);
	}
}

TArray<FDirection> AKKCharacter::RotateDirections(TArray<FDirection> Directions, ERotationDirection RotationDirection)
{
	for(FDirection& InDirection : Directions)
	{
		InDirection = InDirection.Rotate(RotationDirection);
	}

	return Directions;
}

void AKKCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	TextRenderName->SetWorldRotation(FRotator(0.f)); // rotation is handled in text render material

	TextRenderName->SetTextMaterial(UCharacterHelpersSettings::Get()->TextRenderMaterial.LoadSynchronous());
	Platform->SetStaticMesh(UCharacterHelpersSettings::Get()->PlatformMesh.LoadSynchronous());
	
	if(CharacterDataAsset != nullptr)
	{
		CharacterStats = CharacterDataAsset->CharacterStats;
		TextRenderName->SetText(CharacterDataAsset->CharacterName);
		
		UMaterialInstanceDynamic* DynamicPlatformMaterial =  Platform->CreateAndSetMaterialInstanceDynamic(0);
		DynamicPlatformMaterial->SetTextureParameterValue(FName("CharacterTexture"), CharacterDataAsset->CharacterCardTexture);
		
		
		if(CharacterDataAsset->SkeletalMesh && CharacterDataAsset->AnimBlueprint)
		{
			CharacterMesh->SetSkeletalMesh(CharacterDataAsset->SkeletalMesh);
			CharacterMesh->SetAnimInstanceClass(CharacterDataAsset->AnimBlueprint->GeneratedClass);
		}
	}
}

bool AKKCharacter::DefaultAttack(AKKCharacter* TargetCharacter)
{
	if (!DefaultAttackConditions(TargetCharacter) && !HasAuthority())
		return false;
	

	if(true)
	{
		PlayAnimMontage(CharacterDataAsset->AttackMontage);
		
		if(TargetCharacter->GetHealth() <= 0.f)
		{
			KillCharacter(TargetCharacter);
		}
	}
	
	return false;
}

void AKKCharacter::TryUseActiveAbility(int32 Index)
{
	if(CanUseActiveAbility(Index))
	{
		
	}
}

void AKKCharacter::ActiveAbility(int32 Index, TScriptInterface<ISelectableInterface> SelectableObject)
{
	if(CanUseActiveAbility(Index))
	{
		ActiveAbility_Internal(Index, SelectableObject);
		ConsumeActiveAbilityCost(Index);
	}
}

void AKKCharacter::ActiveAbility_Internal(int32 Index, TScriptInterface<ISelectableInterface> SelectableObject)
{
}

void AKKCharacter::ShowActiveAbilityState(bool ReverseState)
{
}

bool AKKCharacter::CanUseActiveAbility(int32 Index)
{
	return GetMana() >= GetActiveAbilityCost(Index);
}

void AKKCharacter::ConsumeActiveAbilityCost(int32 Index)
{
	DecreaseMana(GetActiveAbilityCost(Index));
}

void AKKCharacter::PlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	UAnimInstance * AnimInstance = CharacterMesh->GetAnimInstance();
	AnimInstance->Montage_Play(AnimMontage);
}

void AKKCharacter::KillCharacter(AKKCharacter* TargetCharacter) const
{
	if (HasAuthority() && TargetCharacter->Implements<UBaseInterface>())
	{
		if (AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->EndGameWithWinner(OwningPlayer->PlayerID);
		}
	}
	
	TargetCharacter->CharacterDied();
	GetMap()->RemoveCharacterFromTile(TargetCharacter->GetTilePositionID());
	TargetCharacter->Destroy();
}

void AKKCharacter::DealDamage(AKKCharacter* TargetCharacter, int32 Damage)
{
	if(HasAuthority())
	{
		//todo: Deal Damage 
		if (TargetCharacter->GetHealth() <= 0)
		{
			KillCharacter(TargetCharacter);
		}
	}
}

int32 AKKCharacter::GetStrengthForAttack(AKKCharacter* TargetCharacter)
{
	return GetStrength();
}

bool AKKCharacter::CanBeAttacked(EAttackType AttackType)
{
	return true;
}

int32 AKKCharacter::GetDistanceTo(AKKCharacter* TargetCharacter) const
{
	int32 TargetTileID = TargetCharacter->OwnedTileID;
	int32 OwnerID = OwningPlayer->PlayerID;

	if (TargetCharacter->Implements<UBaseInterface>())
	{
		int32 TileOneID = (OwnerID == 1) ? 17 : 1;
		int32 TileTwoID = (OwnerID == 2) ? 18 : 2;
		
		if (OwnedTileID == TileOneID || OwnedTileID == TileTwoID )
		{
			return 0;
		}

		return MAX_int32;
	}

	FVector2D PositionOne = FVector2D(OwnedTileID / 4, OwnedTileID % 4);
	FVector2D PositionTwo = FVector2D(TargetTileID / 4, TargetTileID % 4);

	//UE_LOG(LogTemp, Warning, TEXT("%d"), static_cast<int32>(FVector2D::Distance(PositionOne, PositionTwo)))

	return FVector2D::Distance(PositionOne, PositionTwo);
}

bool AKKCharacter::IsInLineWith(AKKCharacter* TargetCharacter) const
{
	int32 TargetTileID = TargetCharacter->OwnedTileID;

	bool InLineX = (OwnedTileID / 4) == (TargetTileID / 4);
	bool InLineY = (OwnedTileID % 4) == (TargetTileID % 4);

	return (InLineX || InLineY);
}

bool AKKCharacter::DefaultAttackConditions(AKKCharacter* TargetCharacter)
{
	if(MinAttackConditions(TargetCharacter))
	{
		if(GetDistanceTo(TargetCharacter) <= CharacterStats.MaxAttackRange && IsInLineWith(TargetCharacter))
			return true;
	}
	
	return false;
}

bool AKKCharacter::MinAttackConditions(AKKCharacter* TargetCharacter)
{
	if(TargetCharacter == nullptr || TargetCharacter == this || !IsCharacterOnMap())
		return false;
	
	return (!IsInTheSameTeam(TargetCharacter));
}

AKKMap* AKKCharacter::GetMap() const
{
	return GetWorld()->GetGameState<AKKGameState>()->Map;
}


// Called when the game starts or when spawned
void AKKCharacter::BeginPlay()
{
	Super::BeginPlay();


}

void AKKCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void AKKCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKCharacter, CharacterStats);
	DOREPLIFETIME(AKKCharacter, OwnedTileID);
	DOREPLIFETIME(AKKCharacter, CharacterActions);
	DOREPLIFETIME_CONDITION(AKKCharacter, Direction, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AKKCharacter, OwningPlayer, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AKKCharacter, CharacterID, COND_InitialOnly);
}

/*
UE_LOG(LogTemp, Warning, TEXT("\nHealth : %d \nMana : %d \nDefence : %d \nStrength : %d \n"),
	   CharacterStats.Health,
	   CharacterStats.Mana,
	   CharacterStats.Defence,
	   CharacterStats.Strength);
*/
