// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KKPlayer.generated.h"


class UCameraComponent;
class USpringArmComponent;
class USpotLightComponent;

UCLASS()
class KRZYWEKARTY_API AKKPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKKPlayer();

	FRotator GetCameraRotation() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	USpotLightComponent* SpotLight;
};
