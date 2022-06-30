// Fill out your copyright notice in the Description page of Project Settings.


#include "KKPlayerController.h"
#include "KKCharacter.h"
#include "KKGameMode.h"
#include "KrzyweKarty/UI/CharacterStatsWidget.h"


AKKPlayerController::AKKPlayerController()
{
	bReplicates = true;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	static ConstructorHelpers::FClassFinder<UCharacterStatsWidget>
		ClassFinder(TEXT("/Game/UI/WB_CharacterStatsWidget"));

	if (ClassFinder.Class)
	{
		CharacterStatsWidgetClass = ClassFinder.Class;
	}
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

	InputComponent->BindAction("AddOnPosition0", IE_Pressed, this, &AKKPlayerController::AddOnPosition0);
	InputComponent->BindAction("AddOnPosition1", IE_Pressed, this, &AKKPlayerController::AddOnPosition1);
	InputComponent->BindAction("AddOnPosition2", IE_Pressed, this, &AKKPlayerController::AddOnPosition2);
	InputComponent->BindAction("AddOnPosition3", IE_Pressed, this, &AKKPlayerController::AddOnPosition3);

	InputComponent->BindAction("MoveCardForward", IE_Pressed, this, &AKKPlayerController::MoveForward);
	InputComponent->BindAction("MoveCardBackward", IE_Pressed, this, &AKKPlayerController::MoveBackward);
	InputComponent->BindAction("MoveCardRight", IE_Pressed, this, &AKKPlayerController::MoveRight);
	InputComponent->BindAction("MoveCardLeft", IE_Pressed, this, &AKKPlayerController::MoveLeft);
}

void AKKPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKKPlayerController, SelectedCharacter);
	DOREPLIFETIME(AKKPlayerController, TargetedCharacter);

	DOREPLIFETIME(AKKPlayerController, PlayerID);
	DOREPLIFETIME(AKKPlayerController, MovesCounter)
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

void AKKPlayerController::Server_TraceForSelectedCharacter_Implementation(AKKCharacter* TracedCharacter)
{
	if (AKKCharacter* CardCharacter = Cast<AKKCharacter>(TracedCharacter))
	{
		// if (CardCharacter->OwningPlayer == this)
		// {
		SelectedCharacter = CardCharacter;

		ShowCharacterStats(SelectedCharacter);
		//}
	}
}

void AKKPlayerController::Server_TraceForTargetedCharacter_Implementation(AKKCharacter* TracedCharacter)
{
	if (AKKCharacter* CardCharacter = Cast<AKKCharacter>(TracedCharacter))
	{
		TargetedCharacter = CardCharacter;
	}
}

void AKKPlayerController::Server_AddCharacterToMap_Implementation(int32 TileID)
{
	if (HasAuthority() && SelectedCharacter != nullptr)
	{
		AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode->AddCharacterToMap(SelectedCharacter, TileID, PlayerID))
		{
			MovesCounter++;
		}
		UE_LOG(LogTemp, Warning, TEXT("%d"), MovesCounter);
	}
}

void AKKPlayerController::Server_MoveCharacter_Implementation(EMovementDirection MovementDirection)
{
	if (HasAuthority() && SelectedCharacter != nullptr)
	{
		AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode());
		bool (AKKGameMode::*MoveFunction)(AKKCharacter*, int) = &AKKGameMode::MoveForward;

		switch (MovementDirection)
		{
		case EMD_Forward:
			MoveFunction = &AKKGameMode::MoveForward;
			break;
		case EMD_Backward:
			MoveFunction = &AKKGameMode::MoveBackward;
			break;
		case EMD_Right:
			MoveFunction = &AKKGameMode::MoveRight;
			break;
		case EMD_Left:
			MoveFunction = &AKKGameMode::MoveLeft;
			break;
		default:
			break;
		}

		if ((GameMode->*MoveFunction)(SelectedCharacter, PlayerID))
		{
			MovesCounter++;
		}
		UE_LOG(LogTemp, Warning, TEXT("%d"), MovesCounter);
	}
}

void AKKPlayerController::ShowCharacterStats_Implementation(AKKCharacter* CardCharacter)
{
	if (CharacterStatsWidgetClass)
	{
		CleanupGameViewport();
		
		UCharacterStatsWidget* Widget = CreateWidget<UCharacterStatsWidget>(this, CharacterStatsWidgetClass);

		Widget->ShowStats(CardCharacter);
		Widget->AddToViewport();
	}
}
