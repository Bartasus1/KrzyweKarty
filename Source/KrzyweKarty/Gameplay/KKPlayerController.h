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

UENUM()
enum EMovementDirection
{
	EMD_Forward,
	EMD_Backward,
	EMD_Right,
	EMD_Left
};

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

	UPROPERTY(Replicated)
	int32 MovesCounter;

protected:
	UFUNCTION(Server, Reliable)
	void Server_TraceForSelectedCharacter(AKKCharacter* TracedCharacter);

	UFUNCTION(Server, Reliable)
	void Server_TraceForTargetedCharacter(AKKCharacter* TracedCharacter);

	UFUNCTION(Server, Reliable)
	void Server_AddCharacterToMap(int32 TileID);

	UFUNCTION(Server, Reliable)
	void Server_MoveCharacter(EMovementDirection MovementDirection);

	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private: // Input functions
	FORCEINLINE void SelectCharacter() { Server_TraceForSelectedCharacter(TraceForCharacter()); }
	FORCEINLINE void TargetCharacter() { Server_TraceForTargetedCharacter(TraceForCharacter()); }

	FORCEINLINE void AddOnPosition0() { Server_AddCharacterToMap(0); }
	FORCEINLINE void AddOnPosition1() { Server_AddCharacterToMap(1); }
	FORCEINLINE void AddOnPosition2() { Server_AddCharacterToMap(2); }
	FORCEINLINE void AddOnPosition3() { Server_AddCharacterToMap(3); }

	FORCEINLINE void MoveForward() { Server_MoveCharacter(EMD_Forward); }
	FORCEINLINE void MoveBackward() { Server_MoveCharacter(EMD_Backward); }
	FORCEINLINE void MoveRight() { Server_MoveCharacter(EMD_Right); }
	FORCEINLINE void MoveLeft() { Server_MoveCharacter(EMD_Left); }

	virtual void BeginPlay() override;
};
