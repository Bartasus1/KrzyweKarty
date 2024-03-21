// Fill out your copyright notice in the Description page of Project Settings.



#include "KKCharacter.h"
#include "AbilityActor.h"
#include "Action.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimInstance.h"
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
#include "KrzyweKarty/KrzyweKarty.h"
#include "KrzyweKarty/UI/PlayerHUD.h"

#include "Materials/MaterialInstanceDynamic.h"


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
	CharacterMesh->SetCastShadow(false);
	
	TextRenderName->SetRelativeLocation(FVector(0, 0, 110));
	TextRenderName->SetTextRenderColor(FColor::Red);
	TextRenderName->SetHorizontalAlignment(EHTA_Center);
	TextRenderName->SetWorldSize(18.f);

	SetCollisionResponseToChannel(CharacterTraceChannel, ECR_Block);
}

void AKKCharacter::CharacterDied_Implementation()
{
	OnCharacterDeath.Broadcast();
}

TSubclassOf<AAbilityActor> AKKCharacter::GetAbilityClass(uint8 Index) const
{
	const TSoftClassPtr<AAbilityActor> AbilityClass = CharacterDataAsset->ActiveAbilities[Index].AbilityClass;

	if(!AbilityClass.IsNull())
	{
		return AbilityClass.LoadSynchronous();
	}

	return AAbilityActor::StaticClass();
}

uint8 AKKCharacter::GetTilePositionID() const
{
	return OwnedTileID;
}

void AKKCharacter::OnSelectableHighlighted()
{
	
}

void AKKCharacter::OnSelectableGainFocus()
{
}

void AKKCharacter::OnSelectableLostFocus()
{
}

void AKKCharacter::SetCollisionResponseToChannel(ECollisionChannel CollisionChannel, ECollisionResponse CollisionResponse)
{
	CharacterMesh->SetCollisionResponseToChannel(CollisionChannel, CollisionResponse);
	Platform->SetCollisionResponseToChannel(CollisionChannel, CollisionResponse);
}

void AKKCharacter::OnRep_CharacterStats() const
{
	OnCharacterStatsChanged.Broadcast();
}

TArray<uint8> AKKCharacter::GetPossibleSpawnTiles()
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
		
		DefaultAttackTiles.Add({i, 0}); //cross pattern
		DefaultAttackTiles.Add({0, i});
	}
	
	return DefaultAttackTiles;
}

TArray<AKKTile*> AKKCharacter::GetMoveTiles()
{
	return GetMap()->GetTilesByDirection(this, GetPossibleMoveTiles(), TSP_NoCharacters);
}

uint8 AKKCharacter::GetTopActionWeight()
{
	return CharacterActions;
}


void AKKCharacter::OnTurnEnded()
{
}

void AKKCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(CharacterDataAsset == nullptr)
	{
		return;
	}

	TextRenderName->SetWorldRotation(FRotator(0.f)); // rotation is handled in text render material

	TextRenderName->SetTextMaterial(UCharacterHelpersSettings::Get()->TextRenderMaterial.LoadSynchronous());
	Platform->SetStaticMesh(UCharacterHelpersSettings::Get()->PlatformMesh.LoadSynchronous());
	
	CharacterStats = CharacterDataAsset->CharacterStats;
	TextRenderName->SetText(CharacterDataAsset->CharacterName);
	
	UMaterialInstanceDynamic* DynamicPlatformMaterial =  Platform->CreateAndSetMaterialInstanceDynamic(0);
	DynamicPlatformMaterial->SetTextureParameterValue(FName("CharacterTexture"), CharacterDataAsset->CharacterCardTexture.LoadSynchronous());
	
	if(!CharacterDataAsset->SkeletalMesh.IsNull() && !CharacterDataAsset->AnimationBlueprint.IsNull())
	{
		const UAnimBlueprint* AnimBlueprint = CharacterDataAsset->AnimationBlueprint.LoadSynchronous();
		
		CharacterMesh->SetSkeletalMesh(CharacterDataAsset->SkeletalMesh.LoadSynchronous());
		if(AnimBlueprint) // todo: animBP not loading in packaged game
		{
			CharacterMesh->SetAnimInstanceClass(AnimBlueprint->GeneratedClass);
		}
	}
}

void AKKCharacter::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	if(AKKPlayerController* PlayerControllerOwner = Cast<AKKPlayerController>(NewOwner))
	{
		OwningPlayer = PlayerControllerOwner;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

FAttackResultInfo AKKCharacter::DefaultAttack(AKKCharacter* TargetCharacter)
{
	FAttackResultInfo AttackResultInfo;

	int32 Damage = DefineDamageAmount(TargetCharacter);

	TargetCharacter->ApplyDamageToSelf(Damage, AttackResultInfo);

	if(AttackResultInfo.AttackStatus == EAttackResult::AttackConfirmed)
	{
		PlayAnimMontage(CharacterDataAsset->AttackMontage.LoadSynchronous());
	}
	
	return AttackResultInfo;
}

int32 AKKCharacter::DefineDamageAmount(AKKCharacter* TargetCharacter)
{
	return GetStrength();
}

void AKKCharacter::ApplyDamageToSelf(int32 DamageAmount, FAttackResultInfo& AttackResultInfo)
{
	DealDamage(this, DamageAmount);
}

FCharacterStats AKKCharacter::CalculateCharacterStatsAfterAttack(AKKCharacter* TargetCharacter)
{
	FCharacterStats CalculatedCharacterStats = TargetCharacter->CharacterStats;

	CalculatedCharacterStats.Defence--;
	CalculatedCharacterStats.Health -= (DefineDamageAmount(TargetCharacter)- TargetCharacter->GetDefence());

	return CalculatedCharacterStats;
}

//////////////////////////////////////////////////////////////////////////////////////////////

bool AKKCharacter::CanFinishAbility_Implementation(uint8 Index)
{
	return true;
}

bool AKKCharacter::CanUseAbility_Implementation(uint8 Index)
{
	if(!CharacterDataAsset->ActiveAbilities.IsValidIndex(Index))
	{
		return false;
	}
	
	return GetMana() >= GetActiveAbilityCost(Index);
}

void AKKCharacter::PerformAbility_Implementation(uint8 Index)
{
}

void AKKCharacter::OnBeginAbility_Implementation(uint8 Index)
{
	Client_OnBeginAbilityDelegate(Index);
}

void AKKCharacter::OnFinishAbility_Implementation(uint8 Index)
{
	Client_OnFinishAbilityDelegate(Index);
}

void AKKCharacter::CommitAbilityCost_Implementation(uint8 Index)
{
	DecreaseMana(GetActiveAbilityCost(Index));
}

void AKKCharacter::Client_OnBeginAbilityDelegate_Implementation(uint8 Index)
{
	OnBeginAbilityDelegate.Broadcast(Index);
}

void AKKCharacter::Client_OnFinishAbilityDelegate_Implementation(uint8 Index)
{
	OnFinishAbilityDelegate.Broadcast(Index);
}

///////////////////////////////////////////////////////////////////////////////////////////////

void AKKCharacter::PlayAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	if(AnimMontage)
	{
		if(UAnimInstance * AnimInstance = CharacterMesh->GetAnimInstance())
		{
			AnimInstance->Montage_Play(AnimMontage);
		}
	}
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

void AKKCharacter::DealDamage(AKKCharacter* TargetCharacter, int32 Damage) const
{
	const int32 NewHealth = TargetCharacter->GetHealth() - (Damage - TargetCharacter->GetDefence());
	
	TargetCharacter->SetHealth(NewHealth);
	TargetCharacter->DecreaseDefence();

	OnRep_CharacterStats();

	if(TargetCharacter->GetHealth() <= 0)
	{
		KillCharacter(TargetCharacter); // Need to debate about it -> should it maybe be placed in ApplyDamageToSelf?
	}
}


int32 AKKCharacter::GetDistanceTo(const TScriptInterface<ISelectableInterface>& SelectableInterface) const
{
	if (SelectableInterface.GetObject()->Implements<UBaseInterface>())
	{
		if(GetMap()->CanAttackBase(this))
		{
			return 0;
		}

		return MAX_int32;
	}

	const int32 TargetTileID = SelectableInterface->GetTilePositionID();
	const int32 MapSize = GetMap()->GetMapSize();

	const FVector2D PositionOne = FVector2D(OwnedTileID / MapSize, OwnedTileID % MapSize);
	const FVector2D PositionTwo = FVector2D(TargetTileID / MapSize, TargetTileID % MapSize);

	//UE_LOG(LogTemp, Warning, TEXT("%d"), static_cast<int32>(FVector2D::Distance(PositionOne, PositionTwo)))

	return FVector2D::Distance(PositionOne, PositionTwo);
}

bool AKKCharacter::IsFirstCharacterInLine(AKKCharacter* TargetCharacter)
{
	if(!GetMap()->AreCharactersInLine(this, TargetCharacter))
	{
		return false;
	}

	FIntPoint Position = GetMap()->GetPositionByTileID(GetTilePositionID());
	FIntPoint TargetPosition = GetMap()->GetPositionByTileID(TargetCharacter->GetTilePositionID());
	
	int32 FIntPoint::* SearchDirection = (Position.X != TargetPosition.X) ? &FIntPoint::X : &FIntPoint::Y;
	int32 FIntPoint::* OtherAxis = (Position.X != TargetPosition.X) ? &FIntPoint::Y : &FIntPoint::X;

	for(int32 i = Position.*SearchDirection + 1; i < TargetPosition.*SearchDirection; i++)
	{
		if(const AKKCharacter* Character = GetMap()->GetCharacterByPosition({i, Position.*OtherAxis}))
		{
			if(Character != TargetCharacter)
			{
				return false;
			}
		}
	}
	
	return true;
}

AKKGameState* AKKCharacter::GetGameState() const
{
	return GetWorld()->GetGameState<AKKGameState>();
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
