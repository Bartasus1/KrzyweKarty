// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "KrzyweKarty/Interfaces/SelectableInterface.h"
#include "KKTile.generated.h"

class UTileStatus;
class UBoxComponent;


UCLASS()
class KRZYWEKARTY_API AKKTile : public AActor, public ISelectableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKKTile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* BoxCollision;

	UPROPERTY(ReplicatedUsing="OnRep_TileID", VisibleAnywhere, BlueprintReadOnly)
	int32 TileID;

	UPROPERTY(BlueprintReadOnly)
	UTileStatus* TileStatus;
	
	UFUNCTION()
	void OnRep_TileID();

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	UTextRenderComponent* TextRenderComponent;
#endif

	UFUNCTION(BlueprintCallable)
	virtual int32 GetTilePositionID() const override;

	virtual void OnSelectableHighlighted() override;

	virtual void OnSelectableGainFocus() override;
	virtual void OnSelectableLostFocus() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetTileStatus(UTileStatus* InTileStatus);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UMaterialInstanceDynamic* DynamicInstaceMaterial;
};
