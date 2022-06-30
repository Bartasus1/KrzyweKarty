// Fill out your copyright notice in the Description page of Project Settings.


#include "KKCharacter.h"
#include "KKPlayerController.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"

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

void AKKCharacter::ActiveAbility()
{
}

void AKKCharacter::PassiveAbility()
{
}

void AKKCharacter::InitializeStats()
{
	if (!StatsDataTableHandle.IsNull())
	{
		const FCharacterStats* ReadStats = StatsDataTableHandle.GetRow<FCharacterStats>("");

		MaxCharacterStats = *ReadStats;
		CharacterStats = *ReadStats;

		/*
		UE_LOG(LogTemp, Warning, TEXT("\nHealth : %d \nMana : %d \nDefence : %d \nStrength : %d \n"),
			   CharacterStats.Health,
			   CharacterStats.Mana,
			   CharacterStats.Defence,
			   CharacterStats.Strength);
		*/
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
