// Fill out your copyright notice in the Description page of Project Settings.


#include "KKPlayer.h"

#include "KKCharacter.h"
#include "KKGameMode.h"
#include "KKPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AKKPlayer::AKKPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bUseControllerRotationYaw = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpotLight = CreateDefaultSubobject<USpotLightComponent>("SpotLight");

	SetRootComponent(SpringArm);
	Camera->SetupAttachment(SpringArm);
	SpotLight->SetupAttachment(Camera);

	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 900.f;
	SpringArm->SocketOffset = FVector(0, 0, 200.f);

	Camera->SetRelativeRotation(FRotator(-10, 0, 0));

	SpotLight->SetIntensity(100000.f);
	SpotLight->SetAttenuationRadius(7000.f);
}

// Called when the game starts or when spawned
void AKKPlayer::BeginPlay()
{
	Super::BeginPlay();
}


