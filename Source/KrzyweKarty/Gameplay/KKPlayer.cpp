// Fill out your copyright notice in the Description page of Project Settings.


#include "KKPlayer.h"

#include "KKCharacter.h"
#include "KKPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AKKPlayer::AKKPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");

	SetRootComponent(SpringArm);
	Camera->SetupAttachment(SpringArm);

	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 900.f;
	SpringArm->SocketOffset = FVector(0, 0, 200.f);
	Camera->SetRelativeRotation(FRotator(-10, 0, 0));
}

// Called when the game starts or when spawned
void AKKPlayer::BeginPlay()
{
	Super::BeginPlay();
}


void AKKPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}
