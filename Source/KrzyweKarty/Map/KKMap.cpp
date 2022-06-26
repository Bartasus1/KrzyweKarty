// Fill out your copyright notice in the Description page of Project Settings.


#include "KKMap.h"

// Sets default values
AKKMap::AKKMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MapMesh = CreateDefaultSubobject<UStaticMeshComponent>("MapMesh");

	MapMesh->SetRelativeScale3D(FVector((1.086507, 0.949018, 1)));

	Tiles.Reserve(22);
}

// Called when the game starts or when spawned
void AKKMap::BeginPlay()
{
	Super::BeginPlay();
}

void AKKMap::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKMap, Tiles);
}

// Called every frame
void AKKMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
