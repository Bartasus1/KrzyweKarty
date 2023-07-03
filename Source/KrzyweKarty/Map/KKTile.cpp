// Fill out your copyright notice in the Description page of Project Settings.


#include "KKTile.h"

#include "Components/BoxComponent.h"
#include "KrzyweKarty/Gameplay/KKPlayerController.h"
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
	BoxCollision->SetCollisionResponseToChannel(SelectableTraceChannel, ECR_Block);

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>("TextRender");
	TextRenderComponent->SetupAttachment(BoxCollision);
	
	TextRenderComponent->SetRelativeLocation(FVector(0, 0, 50));

}

void AKKTile::OnRep_TileID()
{
	TextRenderComponent->SetText(FText::FromString(FString::FromInt(TileID)));
}

int32 AKKTile::GetTilePositionID()
{
	return TileID;
}

// Called when the game starts or when spawned
void AKKTile::BeginPlay()
{
	Super::BeginPlay();
}

void AKKTile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AKKTile, TileID, COND_InitialOnly);
}


