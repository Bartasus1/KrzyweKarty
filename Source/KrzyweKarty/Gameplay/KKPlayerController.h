// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "KKPlayerController.generated.h"

/**
 * 
 */
class AKKCharacter;
class UWidgetManagerComponent;


UENUM()
enum EMovementDirection
{
	EMD_Forward,
	EMD_Backward,
	EMD_Right,
	EMD_Left
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterSpawnedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterMoveDelegate, EMovementDirection, Direction);


UCLASS()
class KRZYWEKARTY_API AKKPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AKKPlayerController();

	AKKCharacter* TraceForCharacter();

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	AKKCharacter* SelectedCharacter;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	AKKCharacter* TargetedCharacter;

	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 PlayerID = 1;

	UPROPERTY(ReplicatedUsing="OnRep_TurnChanged", BlueprintReadWrite)
	bool IsMyTurn = true;

	UPROPERTY(BlueprintAssignable)
	FCharacterMoveDelegate OnCharacterMoved;

	UPROPERTY(BlueprintAssignable)
	FCharacterSpawnedDelegate OnCharacterSpawned;

protected:
	UFUNCTION(Server, Reliable)
	void Server_TraceForSelectedCharacter(AKKCharacter* TracedCharacter);

	UFUNCTION(Server, Reliable)
	void Server_TraceForTargetedCharacter(AKKCharacter* TracedCharacter);

	UFUNCTION(Server, Reliable)
	void Server_AddCharacterToMap(int32 TileID);

	UFUNCTION(Server, Reliable)
	void Server_MoveCharacter(EMovementDirection MovementDirection);

	UFUNCTION(Server, Reliable)
	void Server_AttackCharacter();

	UFUNCTION(Server, Reliable)
	void Server_ActiveAbility();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnRep_TurnChanged();

private:
	UFUNCTION(Client, Reliable)
	void ShowCharacterStats(AKKCharacter* CardCharacter);

	UFUNCTION(Client, Reliable)
	void ShowTargetStats(AKKCharacter* CardCharacter);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UWidgetManagerComponent* WidgetManager;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private: // Input functions
	FORCEINLINE void SelectCharacter()	{ Server_TraceForSelectedCharacter(TraceForCharacter()); }
	FORCEINLINE void TargetCharacter()	{ Server_TraceForTargetedCharacter(TraceForCharacter()); }

	FORCEINLINE void AttackCharacter()	{ if (IsMyTurn) Server_AttackCharacter(); }
	FORCEINLINE void ActiveAbility1()	{ if (IsMyTurn) Server_ActiveAbility(); }

	FORCEINLINE void AddOnPosition0() { if (IsMyTurn) Server_AddCharacterToMap(0); }
	FORCEINLINE void AddOnPosition1() { if (IsMyTurn) Server_AddCharacterToMap(1); }
	FORCEINLINE void AddOnPosition2() { if (IsMyTurn) Server_AddCharacterToMap(2); }
	FORCEINLINE void AddOnPosition3() { if (IsMyTurn) Server_AddCharacterToMap(3); }

	FORCEINLINE void MoveForward()	{ if (IsMyTurn) Server_MoveCharacter(EMD_Forward); }
	FORCEINLINE void MoveBackward() { if (IsMyTurn) Server_MoveCharacter(EMD_Backward); }
	FORCEINLINE void MoveRight()	{ if (IsMyTurn) Server_MoveCharacter(EMD_Right); }
	FORCEINLINE void MoveLeft()		{ if (IsMyTurn) Server_MoveCharacter(EMD_Left); }
};
