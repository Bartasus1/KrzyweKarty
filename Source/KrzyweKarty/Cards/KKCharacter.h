// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDataAsset.h"
#include "GameFramework/Actor.h"
#include "KrzyweKarty/Interfaces/SelectableInterface.h"
#include "KKCharacter.generated.h"

struct FDirection;
class AKKPlayerController;
class UCharacterDataAsset;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UTextRenderComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterDiedDelegate);


UCLASS(Abstract)
class KRZYWEKARTY_API AKKCharacter : public AActor, public ISelectableInterface
{

private:
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
	UCharacterDataAsset* CharacterDataAsset;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	AKKPlayerController* OwningPlayer;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	int32 OwnedTileID = -1;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	int32 Direction = 1;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	int32 CharacterID = 0;
	
	UPROPERTY(BlueprintAssignable)
	FCharacterDiedDelegate OnCharacterDeath;
	
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	TArray<FCharacterActionsBase> CharacterActions = { SpawnAction };
protected:
	
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere) // track stats in game
	FCharacterStats CharacterStats;

	UFUNCTION(BlueprintCallable)
	virtual int32 GetTilePositionID() override;

	UFUNCTION(BlueprintCallable)
	virtual TArray<int32> GetPossibleSpawnTiles();
	
	UFUNCTION(BlueprintCallable)
	virtual TArray<FDirection> GetPossibleMoveTiles();

	UFUNCTION(BlueprintCallable)
	virtual TArray<FDirection> GetPossibleAttackTiles();

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual bool DefaultAttack(AKKCharacter* TargetCharacter);

	virtual bool ActiveAbility(AKKCharacter* TargetCharacter) PURE_VIRTUAL(AKKCharacter::ActiveAbility, return false;);
	virtual bool ActiveAbility2(AKKCharacter* TargetCharacter) PURE_VIRTUAL(AKKCharacter::ActiveAbility, return false;);

	UFUNCTION(NetMulticast, Unreliable)
	void PlayAnimMontage(UAnimMontage* AnimMontage);

	UFUNCTION(NetMulticast, Reliable)
	void CharacterDied();


protected:
	//Subclass Sandbox
	void KillCharacter(AKKCharacter* TargetCharacter) const;
	void DealDamage(AKKCharacter* TargetCharacter, int32 Damage);

	int32 GetDistanceTo(AKKCharacter* TargetCharacter) const;
	bool IsInLineWith(AKKCharacter* TargetCharacter) const;
	bool DefaultAttackConditions(AKKCharacter* TargetCharacter, EAttackType AttackType);
	bool MinAttackConditions(AKKCharacter* TargetCharacter, EAttackType AttackType);

public:
	virtual int32 GetStrengthAtDistance(int32 Distance) { return GetStrength(); }
	virtual bool CanBeAttacked(EAttackType AttackType) { return (OwnedTileID != -1); }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& LifetimeProps) const override;

public:
	FORCEINLINE FText GetCharacterName() const { check(CharacterDataAsset); return CharacterDataAsset->CharacterName; }
	
	FORCEINLINE int32 GetHealth()	const { return CharacterStats.Health; }
	FORCEINLINE int32 GetMana()		const { return CharacterStats.Mana; }
	FORCEINLINE int32 GetDefence()	const { return CharacterStats.Defence; }
	FORCEINLINE int32 GetStrength() const { return CharacterStats.Strength; }

	FORCEINLINE int32 GetDefaultHealth()   const { check(CharacterDataAsset);	return CharacterDataAsset->CharacterStats.Health; }
	FORCEINLINE int32 GetDefaultMana()     const { check(CharacterDataAsset);	return CharacterDataAsset->CharacterStats.Mana; }
	FORCEINLINE int32 GetDefaultDefence()  const { check(CharacterDataAsset);	return CharacterDataAsset->CharacterStats.Defence; }
	FORCEINLINE int32 GetDefaultStrength() const { check(CharacterDataAsset);	return CharacterDataAsset->CharacterStats.Strength; }

	FORCEINLINE void SetHealth(int32 NewHealth)		{ CharacterStats.Health = FMath::Clamp(NewHealth, 0, GetDefaultHealth()); }
	FORCEINLINE void SetMana(int32 NewMana)			{ CharacterStats.Mana = FMath::Clamp(NewMana, 0, GetDefaultMana()); }
	FORCEINLINE void SetDefence(int32 NewDefence)	{ CharacterStats.Defence = FMath::Clamp(NewDefence, 0, GetDefaultDefence()); }
	FORCEINLINE void SetStrength(int32 NewStrength) { CharacterStats.Strength = FMath::Clamp(NewStrength, 0, GetDefaultStrength()); }

	FORCEINLINE void DecreaseHealth(int32 InHealth = 1)   { CharacterStats.Health -= FMath::Clamp(InHealth, 0, GetHealth()); }
	FORCEINLINE void DecreaseMana(int32 InMana = 1)		  { CharacterStats.Mana -= FMath::Clamp(InMana, 0, GetMana()); }
	FORCEINLINE void DecreaseDefence(int32 InDefence = 1) { CharacterStats.Defence -= FMath::Clamp(InDefence, 0, GetDefence()); }

	FORCEINLINE void IncreaseHealth(int32 InHealth = 1)		{ CharacterStats.Health += FMath::Clamp(InHealth, 0, GetDefaultHealth() - GetHealth()); }
	FORCEINLINE void IncreaseMana(int32 InMana = 1)			{ CharacterStats.Mana += FMath::Clamp(InMana, 0, GetDefaultMana() - GetMana()); }
	FORCEINLINE void IncreaseDefence(int32 InDefence = 1)	{ CharacterStats.Defence += FMath::Clamp(InDefence, 0, GetDefaultDefence() - GetDefence()); }

	FORCEINLINE int32 GetFirstAbilityManaCost() const
	{ check(&CharacterDataAsset->ActiveAbilities[0]) return CharacterDataAsset->ActiveAbilities[0].ManaCost; }
	FORCEINLINE int32 GetSecondAbilityManaCost() const
	{ check(&CharacterDataAsset->ActiveAbilities[1]) return CharacterDataAsset->ActiveAbilities[1].ManaCost; }

	FORCEINLINE void DecreaseManaForFirstAbility() { DecreaseMana(GetFirstAbilityManaCost()); }
	FORCEINLINE void DecreaseManaForSecondAbility() { DecreaseMana(GetSecondAbilityManaCost()); }


	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsInTheSameTeam(AKKCharacter* TargetCharacter) const { return TargetCharacter->OwningPlayer == OwningPlayer; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsCharacterOnMap() const { return OwnedTileID != -1; }
};
