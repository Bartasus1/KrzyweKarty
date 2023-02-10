// Fill out your copyright notice in the Description page of Project Settings.


#include "KKTile.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AKKTile::AKKTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(BoxCollision);
	
	BoxCollision->SetBoxExtent(FVector(50.f, 40.f, 3.f));
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

}

void AKKTile::HighlightTile(FColor Color)
{
	BoxCollision->SetHiddenInGame(false);
	BoxCollision->ShapeColor = Color;
}

void AKKTile::StopTileHighlight()
{
	BoxCollision->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AKKTile::BeginPlay()
{
	Super::BeginPlay();

	HighlightTile(FColor::Red);
	
}

void AKKTile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKTile, TileID);
}


