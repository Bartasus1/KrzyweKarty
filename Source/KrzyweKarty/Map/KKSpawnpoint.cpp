// Fill out your copyright notice in the Description page of Project Settings.


#include "KKSpawnpoint.h"
#include "KrzyweKarty/Cards//KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKGameMode.h"

// Sets default values
AKKSpawnpoint::AKKSpawnpoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AKKSpawnpoint::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SpawnCards();

		if(AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->OnPlayerJoined.AddUniqueDynamic(this, &AKKSpawnpoint::AssignPlayerToCards);
		}
	}
}

void AKKSpawnpoint::SpawnCards()
{
	FVector StartLocation = GetActorLocation();
	const int Spacing = (ID == 1) ? 100 : - 100;
	constexpr int MaxRowCharacters = 6;

	TArray<FSpawnSet*> SpawnClasses;
	SpawnTable->GetAllRows("", SpawnClasses);

	for (int i = 0; i < SpawnClasses.Num(); i++)
	{
		if (i == MaxRowCharacters)
		{
			StartLocation.X -= 1.2 * Spacing;
			StartLocation.Y = GetActorLocation().Y + Spacing;
		}

		AKKCharacter* SpawnedCharacter = GetWorld()->SpawnActor<AKKCharacter>(SpawnClasses[i]->ClassToSpawn, StartLocation, GetActorRotation());
		SpawnedCharacter->CharacterID = i + 1;

		SpawnedCards.Add(SpawnedCharacter);
		
		StartLocation.Y += Spacing;
	}
}

void AKKSpawnpoint::AssignPlayerToCards()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerID: %d, Accessed Index: %d"), ID, ID-1);
	if(AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
	{
		for (auto & Character : SpawnedCards)
		{
			Character->OwningPlayer = GameMode->GetPlayerController(ID - 1);
		}
	}
	
}

// Called every frame
void AKKSpawnpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
