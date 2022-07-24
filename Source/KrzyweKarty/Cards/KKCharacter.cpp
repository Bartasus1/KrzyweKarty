// Fill out your copyright notice in the Description page of Project Settings.


#include "KKCharacter.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "KrzyweKarty/Gameplay/KKGameMode.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"
#include "KrzyweKarty/Interfaces/BaseInterface.h"

// Sets default values
AKKCharacter::AKKCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	Platform = CreateDefaultSubobject<UStaticMeshComponent>("Platform");
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>("CharacterMesh");
	TextRenderName = CreateDefaultSubobject<UTextRenderComponent>("TextRenderName");

	SetRootComponent(Platform);
	CharacterMesh->SetupAttachment(Platform);
	TextRenderName->SetupAttachment(Platform);

	CharacterMesh->SetRelativeRotation(FRotator(0, -90, 0));
	CharacterMesh->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));
	CharacterMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CharacterMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	CharacterMesh->SetCastShadow(false);

	TextRenderName->SetRelativeLocation(FVector(0, 0, 110));
	TextRenderName->SetTextRenderColor(FColor::Red);
	TextRenderName->SetHorizontalAlignment(EHTA_Center);
	TextRenderName->SetWorldSize(18.f);
	
}

void AKKCharacter::InitializeStats()
{
	check(CharacterDataAsset);
	
	CharacterStats = CharacterDataAsset->CharacterStats;
}


bool AKKCharacter::DefaultAttack(AKKCharacter* TargetCharacter)
{
	if (TargetCharacter == nullptr && TargetCharacter == this)
		return false;

	if (GetDistanceTo(TargetCharacter) > CharacterStats.MaxAttackRange || !IsInLineWith(TargetCharacter))
		return false;

	if (TargetCharacter->CanBeAttacked(EAT_DefaultAttack) && !IsInTheSameTeam(TargetCharacter))
	{
		const int32 Damage = GetStrengthAtDistance(GetDistanceTo(TargetCharacter));
		DealDamage(TargetCharacter, Damage);
		return true;
	}

	return false;
}


void AKKCharacter::KillCharacter(AKKCharacter* TargetCharacter) const
{
	TargetCharacter->OnCharacterDeath.Broadcast();

	if (HasAuthority() && TargetCharacter->Implements<UBaseInterface>())
	{
		if (AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->EndGameWithWinner(OwningPlayer->PlayerID);
		}
	}

	TargetCharacter->Destroy();
}

void AKKCharacter::DealDamage(AKKCharacter* TargetCharacter, int32 Damage)
{
	const int32 NewHealth = TargetCharacter->GetHealth() - (Damage - TargetCharacter->GetDefence());

	TargetCharacter->SetHealth(NewHealth);
	TargetCharacter->DecreaseDefence();

	if (TargetCharacter->GetHealth() <= 0)
	{
		KillCharacter(TargetCharacter);
	}
}


int32 AKKCharacter::GetDistanceTo(AKKCharacter* TargetCharacter) const
{
	int32 TargetTileID = TargetCharacter->OwnedTileID;

	if (TargetCharacter->Implements<UBaseInterface>())
	{
		if (OwnedTileID == 1 || OwnedTileID == 2 || OwnedTileID == 17 || OwnedTileID == 18) //todo: Eliminate bug with attacking enemy base from starting point 
		{
			return 0;
		}

		return  MAX_int32;
	}

	FVector2D PositionOne = FVector2D(OwnedTileID / 4, OwnedTileID % 4);
	FVector2D PositionTwo = FVector2D(TargetTileID / 4, TargetTileID % 4);

	UE_LOG(LogTemp, Warning, TEXT("%d"), static_cast<int32>(FVector2D::Distance(PositionOne, PositionTwo)))

	return FVector2D::Distance(PositionOne, PositionTwo);
}

bool AKKCharacter::IsInLineWith(AKKCharacter* TargetCharacter) const
{
	int32 TargetTileID = TargetCharacter->OwnedTileID;

	bool InLineX = (OwnedTileID / 4) == (TargetTileID / 4);
	bool InLineY = (OwnedTileID % 4) == (TargetTileID % 4);

	return (InLineX || InLineY);
}


// Called when the game starts or when spawned
void AKKCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeStats();
	
	TextRenderName->SetText(GetCharacterName());
	ClientPlayer = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AKKCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ClientPlayer)
	{
		TextRenderName->SetWorldRotation(ClientPlayer->GetControlRotation());
		TextRenderName->AddRelativeRotation(FRotator(0, 180, 0));
	}
}

void AKKCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKCharacter, CharacterStats);
	DOREPLIFETIME(AKKCharacter, OwningPlayer);
	DOREPLIFETIME(AKKCharacter, OwnedTileID);
}

/*
UE_LOG(LogTemp, Warning, TEXT("\nHealth : %d \nMana : %d \nDefence : %d \nStrength : %d \n"),
	   CharacterStats.Health,
	   CharacterStats.Mana,
	   CharacterStats.Defence,
	   CharacterStats.Strength);
*/
