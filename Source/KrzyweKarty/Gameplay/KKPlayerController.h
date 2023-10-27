// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KKPlayerController.generated.h"

class UAction;
class AKKCharacter;
class AKKTile;
class ISelectableInterface;


UCLASS()
class KRZYWEKARTY_API AKKPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AKKPlayerController();
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AKKCharacter* SelectedCharacter = nullptr;

	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 PlayerID = 1;

	UPROPERTY(ReplicatedUsing="OnRep_TurnChanged", BlueprintReadOnly)
	bool bIsMyTurn = true;
	
protected:
	UFUNCTION(BlueprintCallable)
	void ShowCharacterStats(AKKCharacter* CardCharacter);

	UFUNCTION(BlueprintCallable)
	void ShowTargetStats(AKKCharacter* CardCharacter);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<TSubclassOf<UAction>, UAction*> CharacterActions;

public:
	
	UFUNCTION(BlueprintCallable)
	FHitResult CastLineTrace(ECollisionChannel CollisionChannel) const;

	UFUNCTION(BlueprintCallable)
	TScriptInterface<ISelectableInterface> TraceForSelectable(bool bHigherPriority = false) const;
	void UpdateCharacterInActions();

	UFUNCTION(BlueprintCallable)
	bool SelectCharacter();

	UFUNCTION(BlueprintCallable)
	void OnCharacterSelection();
	
	UFUNCTION()
	void OnRep_TurnChanged();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure)
	bool MinMoveRequirements();
};