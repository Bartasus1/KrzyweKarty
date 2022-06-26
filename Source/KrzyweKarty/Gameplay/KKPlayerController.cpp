// Fill out your copyright notice in the Description page of Project Settings.


#include "KKPlayerController.h"
#include "KKCharacter.h"


AKKPlayerController::AKKPlayerController()
{
	bReplicates = true;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AKKPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
}

void AKKPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Turn", this, &APlayerController::AddYawInput);

	InputComponent->BindAction("SelectCharacter", IE_Pressed, this, &AKKPlayerController::SelectCharacter);
	InputComponent->BindAction("TargetCharacter", IE_Pressed, this, &AKKPlayerController::TargetCharacter);
}

void AKKPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKPlayerController, SelectedCharacter);
	DOREPLIFETIME(AKKPlayerController, TargetedCharacter);
}

AKKCharacter* AKKPlayerController::TraceForCharacter()
{
	float Range = 1500;
	FVector Start, Direction;
	DeprojectMousePositionToWorld(Start, Direction);
	
	FHitResult HitResult;
	FVector End = Start + (Direction * Range);
	
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Camera);
	
	if (AKKCharacter* CardCharacter = Cast<AKKCharacter>(HitResult.GetActor()))
	{
		return CardCharacter;
	}

	return nullptr;
}


void AKKPlayerController::Server_TraceForSelectedCharacter_Implementation()
{
	if (AKKCharacter* CardCharacter = Cast<AKKCharacter>(TraceForCharacter()))
	{
		// if (CardCharacter->OwningPlayer == this)
		// {
			SelectedCharacter = CardCharacter;
		//}
	}
}

void AKKPlayerController::Server_TraceForTargetedCharacter_Implementation()
{
	if (AKKCharacter* CardCharacter = Cast<AKKCharacter>(TraceForCharacter()))
	{
		TargetedCharacter = CardCharacter;
	}
}
