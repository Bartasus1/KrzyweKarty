// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KKTile.generated.h"

class UBoxComponent;

UCLASS()
class KRZYWEKARTY_API AKKTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKKTile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* BoxCollision;

	UPROPERTY(Replicated, VisibleAnywhere)
	int32 TileID;

	void HighlightTile(FColor Color);
	void StopTileHighlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
