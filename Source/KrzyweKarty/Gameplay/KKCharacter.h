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

	UPROPERTY(Replicated, BlueprintReadOnly)
	AKKPlayerController* OwningPlayer;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	int32 OwnedTileID = -1;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) // used to store max stats values
	FCharacterStats DefaultCharacterStats;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere) // used to track stats in game
	FCharacterStats CharacterStats;

	/// Data Table Access ///
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	FDataTableRowHandle StatsDataTableHandle;

	void InitializeStats();

public:
	virtual bool DefaultAttack(AKKCharacter* TargetCharacter, int32 Distance);

	//virtual bool ActiveAbility(AKKCharacter* TargetCharacter);
	virtual void ActiveAbility();
	virtual void PassiveAbility();
	//virtual bool PassiveAbility(AKKCharacter* TargetCharacter);

protected:
	void KillCharacter(AKKCharacter* TargetCharacter);
	void DealDamage(AKKCharacter* TargetCharacter);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& LifetimeProps) const override;

public:
	FORCEINLINE int32 GetHealth() const { return CharacterStats.Health; }
	FORCEINLINE int32 GetMana() const { return CharacterStats.Mana; }
	FORCEINLINE int32 GetDefence() const { return CharacterStats.Defence; }
	FORCEINLINE int32 GetStrength() const { return CharacterStats.Strength; }

	FORCEINLINE int32 GetDefaultHealth() const { return DefaultCharacterStats.Health; }
	FORCEINLINE int32 GetDefaultMana() const { return DefaultCharacterStats.Mana; }
	FORCEINLINE int32 GetDefaultDefence() const { return DefaultCharacterStats.Defence; }
	FORCEINLINE int32 GetDefaultStrength() const { return DefaultCharacterStats.Strength; }

	FORCEINLINE void SetHealth(int32 NewHealth) { CharacterStats.Health = NewHealth; }
	FORCEINLINE void SetMana(int32 NewMana) { CharacterStats.Mana = NewMana; }
	FORCEINLINE void SetDefence(int32 NewDefence) { CharacterStats.Defence = NewDefence; }
	FORCEINLINE void SetStrength(int32 NewStrength) { CharacterStats.Strength = NewStrength; }
};
