// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDataAsset.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "KKCharacter.generated.h"

class AKKPlayerController;
class UCharacterDataAsset;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UTextRenderComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterDiedDelegate);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCharacterDataAsset* CharacterDataAsset;

	UPROPERTY(Replicated, BlueprintReadOnly)
	AKKPlayerController* OwningPlayer;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	int32 OwnedTileID = -1;

	UPROPERTY(BlueprintAssignable)
	FCharacterDiedDelegate OnCharacterDeath;

protected:
	
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere) // track stats in game
	FCharacterStats CharacterStats;
	
	void InitializeStats();

public:
	virtual bool DefaultAttack(AKKCharacter* TargetCharacter);

	virtual bool ActiveAbility(AKKCharacter* TargetCharacter) PURE_VIRTUAL(AKKCharacter::ActiveAbility, return false;);
	virtual bool ActiveAbility2(AKKCharacter* TargetCharacter) PURE_VIRTUAL(AKKCharacter::ActiveAbility, return false;);

protected:
	//Subclass Sandbox
	void KillCharacter(AKKCharacter* TargetCharacter) const;
	void DealDamage(AKKCharacter* TargetCharacter, int32 Damage);

	int32 GetDistanceTo(AKKCharacter* TargetCharacter) const;
	bool IsInLineWith(AKKCharacter* TargetCharacter) const;
	bool IsFromSameFraction(AKKCharacter* TargetCharacter);

public:
	virtual int32 GetStrengthAtDistance(int32 Distance) { return GetStrength(); }
	virtual bool CanBeAttacked(EAttackType AttackType) { return (OwnedTileID != -1); }
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& LifetimeProps) const override;

public:
	FORCEINLINE FText GetCharacterName() const { return CharacterDataAsset->CharacterName; }
	
	FORCEINLINE int32 GetHealth() const { return CharacterStats.Health; }
	FORCEINLINE int32 GetMana() const { return CharacterStats.Mana; }
	FORCEINLINE int32 GetDefence() const { return CharacterStats.Defence; }
	FORCEINLINE int32 GetStrength() const { return CharacterStats.Strength; }

	FORCEINLINE int32 GetDefaultHealth() const { return CharacterDataAsset->CharacterStats.Health; }
	FORCEINLINE int32 GetDefaultMana() const { return CharacterDataAsset->CharacterStats.Mana; }
	FORCEINLINE int32 GetDefaultDefence() const { return CharacterDataAsset->CharacterStats.Defence; }
	FORCEINLINE int32 GetDefaultStrength() const { return CharacterDataAsset->CharacterStats.Strength; }

	FORCEINLINE void SetHealth(int32 NewHealth) { CharacterStats.Health = FMath::Clamp(NewHealth, 0, GetDefaultHealth()); }
	FORCEINLINE void SetMana(int32 NewMana) { CharacterStats.Mana = FMath::Clamp(NewMana, 0, GetDefaultMana()); }
	FORCEINLINE void SetDefence(int32 NewDefence) { CharacterStats.Defence = FMath::Clamp(NewDefence, 0, GetDefaultDefence()); }
	FORCEINLINE void SetStrength(int32 NewStrength) { CharacterStats.Strength = FMath::Clamp(NewStrength, 0, GetDefaultStrength()); }

	FORCEINLINE void DecreaseHealth(int32 InHealth = 1) { CharacterStats.Health -= FMath::Clamp(InHealth, 0, GetHealth()); }
	FORCEINLINE void DecreaseMana(int32 InMana = 1) { CharacterStats.Mana -= FMath::Clamp(InMana, 0, GetMana()); }
	FORCEINLINE void DecreaseDefence(int32 InDefence = 1) { CharacterStats.Defence -= FMath::Clamp(InDefence, 0, GetDefence()); }

	FORCEINLINE void IncreaseHealth(int32 InHealth = 1) { CharacterStats.Health += FMath::Clamp(InHealth, 0, GetDefaultHealth() - GetHealth()); }
	FORCEINLINE void IncreaseMana(int32 InMana = 1) { CharacterStats.Mana += FMath::Clamp(InMana, 0, GetDefaultMana() - GetMana()); }
	FORCEINLINE void IncreaseDefence(int32 InDefence = 1) { CharacterStats.Defence += FMath::Clamp(InDefence, 0, GetDefaultDefence() - GetDefence()); }
};
