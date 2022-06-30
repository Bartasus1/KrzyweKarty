// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KKPlayer.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "KKCharacter.generated.h"

class AKKPlayerController;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UTextRenderComponent;


USTRUCT(BlueprintType)
struct FCharacterStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Defence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAttackRange = 1;
};


UCLASS(Abstract)
class KRZYWEKARTY_API AKKCharacter : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKKCharacter();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Platform;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(EditDefaultsOnly)
	UTextRenderComponent* TextRenderName;

	///////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterName;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere) // used to store max stats values
	FCharacterStats MaxCharacterStats;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere) // used to track stats in game
	FCharacterStats CharacterStats;

	UPROPERTY(Replicated, BlueprintReadWrite)
	AKKPlayerController* OwningPlayer;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	int32 OwnedTileID = -1;

	UFUNCTION(BlueprintCallable)
	virtual void ActiveAbility();

	UFUNCTION(BlueprintCallable)
	virtual void PassiveAbility();

protected:
	/// Data Table Access ///
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	FDataTableRowHandle StatsDataTableHandle;

	UFUNCTION(CallInEditor)
	void InitializeStats();
	/////////////////////////


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& LifetimeProps) const override;
};
