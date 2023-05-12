// Fill out your copyright notice in the Description page of Project Settings.


#include "KKPlayerController.h"
#include "KKGameMode.h"
#include "KrzyweKarty/Cards/KKCharacter.h"
#include "KrzyweKarty/Map/KKTile.h"
#include "KrzyweKarty/UI/PlayerHUD.h"

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

	PlayerCameraManager->ViewPitchMin = -70;
	PlayerCameraManager->ViewPitchMax = 10;
	
}

void AKKPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(AKKCharacter* TracedCharacter = TraceForCharacter())
	{
		ShowCharacterStats(TracedCharacter);
	}
}

void AKKPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Turn", this, &APlayerController::AddYawInput);
	InputComponent->BindAxis("LookUp", this, &AKKPlayerController::AddPitchInput);

	InputComponent->BindAction("SelectCharacter", IE_Pressed, this, &AKKPlayerController::SelectCharacter);
	InputComponent->BindAction("TargetCharacter", IE_Pressed, this, &AKKPlayerController::TargetCharacter);

	InputComponent->BindAction("Attack", IE_Pressed, this, &AKKPlayerController::AttackCharacter);
	InputComponent->BindAction("ActiveAbility1", IE_Pressed, this, &AKKPlayerController::ActiveAbility1);

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
	DOREPLIFETIME(AKKPlayerController, bIsMyTurn);
}

FHitResult AKKPlayerController::CastLineTrace(ECollisionChannel CollisionChannel) const
{
	float Range = 1500;
 	FVector Start, Direction;
 	DeprojectMousePositionToWorld(Start, Direction);
 
 	FHitResult HitResult;
 	FVector End = Start + (Direction * Range);
 
 	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, CollisionChannel);
	
	return HitResult;
}


AKKCharacter* AKKPlayerController::TraceForCharacter() const
{
	return Cast<AKKCharacter>(CastLineTrace(CharacterChannel).GetActor());
}

AKKTile* AKKPlayerController::TraceForPlatform() const
{
	return Cast<AKKTile>(CastLineTrace(PlatformChannel).GetActor());
}

void AKKPlayerController::Server_TraceForSelectedCharacter_Implementation(AKKCharacter* TracedCharacter)
{
	if (TracedCharacter != nullptr && TracedCharacter->OwningPlayer == this)
	{
		SelectedCharacter = TracedCharacter;

		ShowCharacterStats(SelectedCharacter);
	}
}

void AKKPlayerController::Server_TraceForTargetedCharacter_Implementation(AKKCharacter* TracedCharacter)
{
	TargetedCharacter = TracedCharacter;
	
	if(TargetedCharacter != nullptr)
	{
		ShowTargetStats(TracedCharacter);
	}
}

void AKKPlayerController::Server_AttackCharacter_Implementation()
{
	if (!SelectedCharacter || !TargetedCharacter)
		return;
	
	if(AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->PerformCharacterAttack(SelectedCharacter, TargetedCharacter);
	}
}

void AKKPlayerController::Server_ActiveAbility_Implementation()
{
	if (!SelectedCharacter)
		return;
	
	if(AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->PerformCharacterAbility(SelectedCharacter, TargetedCharacter);
	}
}

void AKKPlayerController::Server_AddCharacterToMap_Implementation(int32 TileID)
{
	if (!SelectedCharacter)
		return;
	
	if (AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->AddCharacterToMap(SelectedCharacter, TileID, PlayerID);
		OnCharacterSpawned.Broadcast();
	}
}

void AKKPlayerController::Server_MoveCharacter_Implementation(EMovementDirection MovementDirection)
{
	if (!SelectedCharacter)
		return;
	
	if (AKKGameMode* GameMode = Cast<AKKGameMode>(GetWorld()->GetAuthGameMode()))
	{
		OnCharacterMoved.Broadcast(MovementDirection);

		GameMode->MoveCharacter(SelectedCharacter, MovementDirection, PlayerID);
	}
}

void AKKPlayerController::ShowCharacterStats_Implementation(AKKCharacter* CardCharacter)
{
	GetHUD<APlayerHUD>()->ShowCharacterStats(CardCharacter, true);
}

void AKKPlayerController::ShowTargetStats_Implementation(AKKCharacter* CardCharacter)
{
	GetHUD<APlayerHUD>()->ShowCharacterStats(CardCharacter, false);
}

void AKKPlayerController::OnRep_TurnChanged()
{
	if(GetHUD())
	{
		GetHUD<APlayerHUD>()->OnTurnChange(bIsMyTurn);
	}
}
