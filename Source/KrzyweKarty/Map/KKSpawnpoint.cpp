// Fill out your copyright notice in the Description page of Project Settings.


#include "KKSpawnpoint.h"

#include "KKMap.h"
#include "Kismet/GameplayStatics.h"
#include "KrzyweKarty/Cards//KKCharacter.h"
#include "KrzyweKarty/Gameplay/KKGameMode.h"
#include "KrzyweKarty/Gameplay/KKGameState.h"

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
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AKKSpawnpoint::SpawnCards);

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

	for (int i = 0; i < SpawnClasses.Num() - 1; i++)
	{
		if (i == MaxRowCharacters)
		{
			StartLocation.X -= 1.2 * Spacing;
			StartLocation.Y = GetActorLocation().Y + Spacing;
		}

		AKKCharacter* SpawnedCharacter = GetWorld()->SpawnActor<AKKCharacter>(SpawnClasses[i]->ClassToSpawn, StartLocation, GetActorRotation());
		SpawnedCharacter->CharacterID = i + 1;
		SpawnedCharacter->Direction = (ID == 1) ? 1 : -1;

		SpawnedCards.Add(SpawnedCharacter);
		
		StartLocation.Y += Spacing;
	}

	AKKGameState* GameState = Cast<AKKGameState>(UGameplayStatics::GetGameState(this));
	AKKCharacter* BaseCharacter = GetWorld()->SpawnActor<AKKCharacter>(SpawnClasses[SpawnClasses.Num() - 1]->ClassToSpawn, StartLocation, GetActorRotation() + FRotator(0.f, 90.f, 0.f));

	GameState->Map->SetFractionBase(ID, BaseCharacter);
	SpawnedCards.Add(BaseCharacter);
	
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
