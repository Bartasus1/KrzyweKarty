// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "KKPlayerController.generated.h"

class ALocalMapManager;
class AKKMap;
class UTileStatus;
class UPlayerInputDataAsset;
class UAction;
class AKKCharacter;
class AKKTile;
class ISelectableInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSelectableSelected);

UCLASS()
class KRZYWEKARTY_API AKKPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AKKPlayerController();
	
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	AKKCharacter* SelectedCharacter = nullptr;

	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 PlayerID = 1;

	UPROPERTY(ReplicatedUsing="OnRep_TurnChanged", BlueprintReadOnly)
	bool bIsMyTurn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SelectionInputTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayerInputDataAsset* PlayerInputDataAsset;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FSelectableSelected OnSelectableSelected;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<TSubclassOf<UAction>, UAction*> CharacterActions;
	
protected:
	UFUNCTION(BlueprintCallable)
	void ShowCharacterStats(AKKCharacter* CardCharacter);

	UFUNCTION(BlueprintCallable)
	void ShowTargetStats(AKKCharacter* CardCharacter);

public:
	
	UFUNCTION(BlueprintCallable)
	FHitResult CastLineTrace(ECollisionChannel CollisionChannel) const;

	UFUNCTION(BlueprintCallable)
	TScriptInterface<ISelectableInterface> TraceForCollisionChannel(ECollisionChannel CollisionChannel) const;

	UFUNCTION(BlueprintCallable)
	AKKCharacter* TraceForCharacter() const;

	UFUNCTION(BlueprintCallable)
	AKKCharacter* SelectCharacter();
	
	UFUNCTION()
	void OnRep_TurnChanged();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void OnCharacterSelection_Server(AKKCharacter* InCharacter);

	UFUNCTION(Client, Reliable)
	void ShowTilesForAction(const TArray<AKKTile*>& InTiles, UTileStatus* TileStatus);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure)
	bool MinMoveRequirements() const;

private:

	AKKMap* GetMap() const;
	
	TScriptInterface<ISelectableInterface> CachedTracedSelectableInterface;
};