// Fill out your copyright notice in the Description page of Project Settings.


#include "KKPlayer.h"
#include "KKGameMode.h"
#include "KKPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "KrzyweKarty/Cards/KKCharacter.h"

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

void AKKPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &AKKPlayer::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &AKKPlayer::ZoomOut);
}

// Called when the game starts or when spawned
void AKKPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AKKPlayer::ZoomIn()
{
	if (Camera->FieldOfView > 45)
	{
		float FOV = Camera->FieldOfView - 5;
		Camera->SetFieldOfView(FOV);
	}
}

void AKKPlayer::ZoomOut()
{
	if (Camera->FieldOfView < 130)
	{
		float FOV = Camera->FieldOfView + 5;
		Camera->SetFieldOfView(FOV);
	}
}
