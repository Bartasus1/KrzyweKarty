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
	CharacterMesh->CastShadow = false;


	TextRenderName->SetRelativeLocation(FVector(0, 0, 110));

	InitializeStats();
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
	}
}

// Called when the game starts or when spawned
void AKKCharacter::BeginPlay()
{
	Super::BeginPlay();

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
}
