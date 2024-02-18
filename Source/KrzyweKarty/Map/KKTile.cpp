// Fill out your copyright notice in the Description page of Project Settings.


#include "KKTile.h"
#include "Components/BoxComponent.h"
#include "KrzyweKarty/KrzyweKarty.h"
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

#if WITH_EDITORONLY_DATA
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>("TextRender");
	TextRenderComponent->SetupAttachment(BoxCollision);
	
	TextRenderComponent->SetRelativeLocation(FVector(0, 0, 50));
#endif
}

void AKKTile::OnRep_TileID()
{
#if WITH_EDITORONLY_DATA
	TextRenderComponent->SetText(FText::FromString(FString::FromInt(TileID)));
#endif
}

int32 AKKTile::GetTilePositionID() const
{
	return TileID;
}

void AKKTile::OnSelectableHighlighted()
{
	BoxCollision->SetCollisionResponseToChannel(PriorityTraceChannel, ECR_Block);
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


