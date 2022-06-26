// Fill out your copyright notice in the Description page of Project Settings.


#include "KKCharacter.h"
#include "KKPlayerController.h"
#include "Components/WidgetComponent.h"
#include "KrzyweKarty/UI/CharacterNameWidget.h"


// Sets default values
AKKCharacter::AKKCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Platform = CreateDefaultSubobject<UStaticMeshComponent>("Platform");
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>("CharacterMesh");
	CharacterNameWidget = CreateDefaultSubobject<UWidgetComponent>("CharacterNameWidget");

	SetRootComponent(Platform);
	CharacterMesh->SetupAttachment(Platform);
	CharacterNameWidget->SetupAttachment(Platform);

	CharacterMesh->SetRelativeRotation(FRotator(0, -90, 0));
	CharacterMesh->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));
	CharacterMesh->CastShadow = false;


	CharacterNameWidget->SetRelativeLocation(FVector(0, 0, 110));
	CharacterNameWidget->SetWidgetClass(UCharacterNameWidget::StaticClass());
	CharacterNameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	

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

	if (UCharacterNameWidget* NameWidget = Cast<UCharacterNameWidget>(CharacterNameWidget->GetWidget()))
	{
		NameWidget->CharacterName = CharacterName;
	}
}

void AKKCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKCharacter, CharacterStats);
	DOREPLIFETIME(AKKCharacter, OwningPlayer);
}
