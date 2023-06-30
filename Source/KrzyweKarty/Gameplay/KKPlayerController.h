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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterSpawnedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterMoveDelegate, EMovementDirection, Direction);


UCLASS()
class KRZYWEKARTY_API AKKPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AKKPlayerController();
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AKKCharacter* SelectedCharacter;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AKKCharacter* TargetedCharacter;

	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 PlayerID = 1;

	UPROPERTY(ReplicatedUsing="OnRep_TurnChanged", BlueprintReadOnly)
	bool bIsMyTurn = true;

	UPROPERTY(BlueprintAssignable)
	FCharacterMoveDelegate OnCharacterMoved;

	UPROPERTY(BlueprintAssignable)
	FCharacterSpawnedDelegate OnCharacterSpawned;

public:
	
	UFUNCTION(BlueprintCallable)
	FHitResult CastLineTrace(ECollisionChannel CollisionChannel) const;

	UFUNCTION(BlueprintCallable)
	TScriptInterface<ISelectableInterface> TraceForSelectable(bool bHigherPriority = false) const;

protected:
	// UFUNCTION(Server, Reliable, BlueprintCallable)
	// void Server_TraceForSelectedCharacter(AKKCharacter* TracedCharacter);
	//
	// UFUNCTION(Server, Reliable, BlueprintCallable)
	// void Server_TraceForTargetedCharacter(AKKCharacter* TracedCharacter);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AddCharacterToMap(int32 TileID);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_MoveCharacter(EMovementDirection MovementDirection);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AttackCharacter();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_ActiveAbility();

public:
	UFUNCTION()
	void OnRep_TurnChanged();
	

private:
	UFUNCTION(BlueprintCallable)
	void ShowCharacterStats(AKKCharacter* CardCharacter);

	UFUNCTION(BlueprintCallable)
	void ShowTargetStats(AKKCharacter* CardCharacter);

	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	//virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private: // Input functions

	FORCEINLINE void AttackCharacter()	{ if (bIsMyTurn) Server_AttackCharacter(); }
	FORCEINLINE void ActiveAbility1()	{ if (bIsMyTurn) Server_ActiveAbility(); }

	FORCEINLINE void AddOnPosition0() { if (bIsMyTurn) Server_AddCharacterToMap(0); }
	FORCEINLINE void AddOnPosition1() { if (bIsMyTurn) Server_AddCharacterToMap(1); }
	FORCEINLINE void AddOnPosition2() { if (bIsMyTurn) Server_AddCharacterToMap(2); }
	FORCEINLINE void AddOnPosition3() { if (bIsMyTurn) Server_AddCharacterToMap(3); }

	FORCEINLINE void MoveForward()	{ if (bIsMyTurn) Server_MoveCharacter(EMD_Forward); }
	FORCEINLINE void MoveBackward() { if (bIsMyTurn) Server_MoveCharacter(EMD_Backward); }
	FORCEINLINE void MoveRight()	{ if (bIsMyTurn) Server_MoveCharacter(EMD_Right); }
	FORCEINLINE void MoveLeft()		{ if (bIsMyTurn) Server_MoveCharacter(EMD_Left); }
};
