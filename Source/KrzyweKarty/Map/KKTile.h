// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "KrzyweKarty/Interfaces/SelectableInterface.h"
#include "KKTile.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum ETileColor
{
	None,
	Red,
	Blue,
	Yellow
};

UCLASS()
class KRZYWEKARTY_API AKKTile : public AActor, public ISelectableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKKTile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* BoxCollision;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 TileID;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* TextRenderComponent;

	UFUNCTION(BlueprintCallable)
	virtual int32 GetTilePositionID() override;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Client_SetTileColor(ETileColor TileColor);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetTileColor(ETileColor TileColor);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
