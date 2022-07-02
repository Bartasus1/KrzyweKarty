// Fill out your copyright notice in the Description page of Project Settings.


#include "KKCharacter.h"

#include "KKGameMode.h"
#include "KKPlayerController.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
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
	if (!StatsDataTableHandle.IsNull())
	{
		const FCharacterStats* ReadStats = StatsDataTableHandle.GetRow<FCharacterStats>("");

		DefaultCharacterStats = *ReadStats;
		CharacterStats = *ReadStats;
	}
}

bool AKKCharacter::DefaultAttack(AKKCharacter* TargetCharacter, int32 Distance)
{
	DealDamage(TargetCharacter);
	return true;
}

void AKKCharacter::ActiveAbility()
{
}

// bool AKKCharacter::ActiveAbility(AKKCharacter* TargetCharacter)
// {
// 	return true;
// }
//
// bool AKKCharacter::PassiveAbility(AKKCharacter* TargetCharacter)
// {
// 	return true;
// }

void AKKCharacter::PassiveAbility()
{
}

void AKKCharacter::KillCharacter(AKKCharacter* TargetCharacter)
{
	if (HasAuthority() && TargetCharacter->Implements<UBaseInterface>())
	{
		if (AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->EndGameWithWinner(OwningPlayer->PlayerID);
		}
	}

	TargetCharacter->Destroy();
}

void AKKCharacter::DealDamage(AKKCharacter* TargetCharacter)
{
	const int32 NewHealth = TargetCharacter->GetHealth() - (GetStrength() - TargetCharacter->GetDefence());

	TargetCharacter->SetHealth(FMath::Clamp(NewHealth, 0, TargetCharacter->GetHealth()));
	TargetCharacter->SetDefence(FMath::Clamp(TargetCharacter->GetDefence() - 1, 0, TargetCharacter->GetDefence()));

	if (TargetCharacter->GetHealth() <= 0)
	{
		KillCharacter(TargetCharacter);
	}
}

// Called when the game starts or when spawned
void AKKCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeStats();

	TextRenderName->SetText(CharacterName);
	OwningPlayer = Cast<AKKPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AKKCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (OwningPlayer)
	{
		TextRenderName->SetWorldRotation(OwningPlayer->GetControlRotation());
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
