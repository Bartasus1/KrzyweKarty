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
class UCharacterStatsWidget;

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
	int32 PlayerID = 1;

	UPROPERTY(Replicated)
	bool CanMove = true;


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

private:
	UFUNCTION(Client, Reliable)
	void ShowCharacterStats(AKKCharacter* CardCharacter);

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UCharacterStatsWidget> CharacterStatsWidgetClass;

	UPROPERTY()
	UCharacterStatsWidget* StatsWidget;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private: // Input functions
	FORCEINLINE void SelectCharacter() { Server_TraceForSelectedCharacter(TraceForCharacter()); }
	FORCEINLINE void TargetCharacter() { Server_TraceForTargetedCharacter(TraceForCharacter()); }
	
	FORCEINLINE void AttackCharacter() { if(CanMove) Server_AttackCharacter(); }

	FORCEINLINE void AddOnPosition0() { if(CanMove) Server_AddCharacterToMap(0); }
	FORCEINLINE void AddOnPosition1() { if(CanMove) Server_AddCharacterToMap(1); }
	FORCEINLINE void AddOnPosition2() { if(CanMove) Server_AddCharacterToMap(2); }
	FORCEINLINE void AddOnPosition3() { if(CanMove) Server_AddCharacterToMap(3); }

	FORCEINLINE void MoveForward()	{ if(CanMove) Server_MoveCharacter(EMD_Forward); }
	FORCEINLINE void MoveBackward() { if(CanMove) Server_MoveCharacter(EMD_Backward); }
	FORCEINLINE void MoveRight()	{ if(CanMove) Server_MoveCharacter(EMD_Right); }
	FORCEINLINE void MoveLeft()		{ if(CanMove) Server_MoveCharacter(EMD_Left); }
};
