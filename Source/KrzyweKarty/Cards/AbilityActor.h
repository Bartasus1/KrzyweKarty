// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityActor.generated.h"

class UAbilityAction;
class AKKCharacter;

UCLASS()
class KRZYWEKARTY_API AAbilityActor : public AActor
{
	GENERATED_BODY()

public:
	AAbilityActor();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void BeginAbility();

	UFUNCTION(Client, Reliable)
	void OnBeginAbility_Client();

	UFUNCTION(Client, Reliable)
	void OnFinishAbility_Client();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnBeginAbility();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnFinishAbility();
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ConfirmAbility();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void AbortAbility();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, meta=(ExposeOnSpawn))
	AKKCharacter* Character;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, meta=(ExposeOnSpawn))
	uint8 AbilityIndex = 0;

	UPROPERTY(VisibleAnywhere)
	UAbilityAction* AbilityAction;
};
