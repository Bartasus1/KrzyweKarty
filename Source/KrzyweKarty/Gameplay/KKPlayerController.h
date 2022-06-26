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

UCLASS()
class KRZYWEKARTY_API AKKPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AKKPlayerController();
	
	virtual void BeginPlay() override;

	AKKCharacter* TraceForCharacter();

	UPROPERTY(Replicated, BlueprintReadWrite)
	AKKCharacter* SelectedCharacter;

	UPROPERTY(Replicated, BlueprintReadWrite)
	AKKCharacter* TargetedCharacter;

protected:
	UFUNCTION(Server, Reliable)
	void Server_TraceForSelectedCharacter();

	UFUNCTION(Server, Reliable)
	void Server_TraceForTargetedCharacter();

	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private: // Input functions
	FORCEINLINE void SelectCharacter() { Server_TraceForSelectedCharacter(); }
	FORCEINLINE void TargetCharacter() { Server_TraceForTargetedCharacter(); }
};
