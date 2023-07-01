// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KKPlayerController.generated.h"


class AKKCharacter;
class AKKTile;
class ISelectableInterface;

#define SelectableTraceChannel ECC_GameTraceChannel1
#define PriorityTraceChannel ECC_GameTraceChannel2

UENUM()
enum EMovementDirection
{
	EMD_Forward	=   4	UMETA(DisplayName = "Forward"),
	EMD_Backward = -4	UMETA(DisplayName = "Backward"),
	EMD_Right =     1	UMETA(DisplayName = "Right"),
	EMD_Left =     -1	UMETA(DisplayName = "Left")
};

UCLASS()
class KRZYWEKARTY_API AKKPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AKKPlayerController();
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AKKCharacter* SelectedCharacter;

	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 PlayerID = 1;

	UPROPERTY(ReplicatedUsing="OnRep_TurnChanged", BlueprintReadOnly)
	bool bIsMyTurn = true;
	
private:
	UFUNCTION(BlueprintCallable)
	void ShowCharacterStats(AKKCharacter* CardCharacter);

	UFUNCTION(BlueprintCallable)
	void ShowTargetStats(AKKCharacter* CardCharacter);

public:
	
	UFUNCTION(BlueprintCallable)
	FHitResult CastLineTrace(ECollisionChannel CollisionChannel) const;

	UFUNCTION(BlueprintCallable)
	TScriptInterface<ISelectableInterface> TraceForSelectable(bool bHigherPriority = false) const;
	
	UFUNCTION()
	void OnRep_TurnChanged();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure)
	bool MinMoveRequirements();
};
