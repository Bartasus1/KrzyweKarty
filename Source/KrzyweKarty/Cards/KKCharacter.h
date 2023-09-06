// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CharacterDataAsset.h"
#include "GameFramework/Actor.h"
#include "KrzyweKarty/Interfaces/SelectableInterface.h"
#include "KrzyweKarty/Map/KKMap.h"
#include "KKCharacter.generated.h"

struct FMapCell;
enum ERotationDirection : int;
class AKKTile;
class AKKMap;
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
	TArray<TEnumAsByte<EMovementType>> CharacterActions;
protected:
	
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere) // track stats in game
	FCharacterStats CharacterStats;

	// Interaction with the Map
	UFUNCTION(BlueprintCallable)
	virtual TArray<int32> GetPossibleSpawnTiles();
	
	UFUNCTION(BlueprintCallable)
	virtual TArray<FDirection> GetPossibleMoveTiles();

	UFUNCTION(BlueprintCallable)
	virtual TArray<FDirection> GetPossibleAttackTiles();

	UFUNCTION(BlueprintCallable)
	virtual void HighlightDefaultAttackTiles();
	
	UFUNCTION(BlueprintCallable)
	virtual void HighlightMoveTiles();

	UFUNCTION(BlueprintCallable)
	TArray<FDirection> RotateDirections(TArray<FDirection> Directions, ERotationDirection RotationDirection);

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Actions
	virtual bool DefaultAttack(AKKCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable)
	void TryUseActiveAbility(int32 Index);

	UFUNCTION(BlueprintCallable)
	virtual void ActiveAbility(int32 Index, TScriptInterface<ISelectableInterface> SelectableObject);

	UFUNCTION(BlueprintCallable)
	virtual void ShowActiveAbilityState(bool ReverseState = false);
	
	UFUNCTION(BlueprintCallable)
	virtual bool CanUseActiveAbility(int32 Index);
	UFUNCTION(BlueprintCallable)
	virtual void ConsumeActiveAbilityCost(int32 Index);

	//protected:
	virtual void ActiveAbility_Internal(int32 Index, TScriptInterface<ISelectableInterface> SelectableObject); 

	UFUNCTION(BlueprintCallable)
	virtual int32 GetTilePositionID() override;

	UFUNCTION(BlueprintCallable)
	virtual TArray<AKKTile*> GetMoveTiles();

	UFUNCTION(BlueprintCallable)
	virtual TArray<AKKTile*> GetAttackTiles();

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
	bool DefaultAttackConditions(AKKCharacter* TargetCharacter);
	bool MinAttackConditions(AKKCharacter* TargetCharacter);

	//virtual void NotifyAttackBegin(EAttackType AttackType);
	//virtual void NotifyAttackEnd(EAttackType AttackType);

	AKKMap* GetMap() const;

public:
	virtual int32 GetStrengthForAttack(AKKCharacter* TargetCharacter);
	virtual bool CanBeAttacked(EAttackType AttackType);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& LifetimeProps) const override;

	

public:
	FORCEINLINE FText GetCharacterName() const { return CharacterDataAsset->CharacterName; }
	
	FORCEINLINE int32 GetHealth()	const { return CharacterStats.Health; }
	FORCEINLINE int32 GetMana()		const { return CharacterStats.Mana; }
	FORCEINLINE int32 GetDefence()	const { return CharacterStats.Defence; }
	FORCEINLINE int32 GetStrength() const { return CharacterStats.Strength; }

	FORCEINLINE int32 GetDefaultHealth()   const { return CharacterDataAsset->CharacterStats.Health; }
	FORCEINLINE int32 GetDefaultMana()     const { return CharacterDataAsset->CharacterStats.Mana; }
	FORCEINLINE int32 GetDefaultDefence()  const { return CharacterDataAsset->CharacterStats.Defence; }
	FORCEINLINE int32 GetDefaultStrength() const { return CharacterDataAsset->CharacterStats.Strength; }

	FORCEINLINE void SetHealth(int32 NewHealth) 	{ CharacterStats.Health		=	FMath::Clamp<int32>(NewHealth, 0, GetDefaultHealth()); }
	FORCEINLINE void SetMana(int32 NewMana)			{ CharacterStats.Mana		=	FMath::Clamp<int32>(NewMana, 0, GetDefaultHealth()); }
	FORCEINLINE void SetDefence(int32 NewDefence)	{ CharacterStats.Defence	=	FMath::Clamp<int32>(NewDefence, 0, GetDefaultHealth()); }
	FORCEINLINE void SetStrength(int32 NewStrength) { CharacterStats.Strength	=	FMath::Clamp<int32>(NewStrength, 0, GetDefaultHealth()); }

	FORCEINLINE void DecreaseHealth(int32 InHealth = 1)   { SetHealth(GetHealth() - InHealth); }
	FORCEINLINE void DecreaseMana(int32 InMana = 1)		  { SetMana(GetMana() - InMana); }
	FORCEINLINE void DecreaseDefence(int32 InDefence = 1) { SetDefence(GetDefence() - InDefence); }

	FORCEINLINE void IncreaseHealth(int32 InHealth = 1)		 { SetHealth(GetHealth() + InHealth); }
	FORCEINLINE void IncreaseMana(int32 InMana = 1)			 { SetMana(GetMana() + InMana); }
	FORCEINLINE void IncreaseDefence(int32 InDefence = 1)	 { SetDefence(GetDefence() + InDefence); }

	FORCEINLINE int32 GetActiveAbilityCost(int32 Index) const
	{ check(&CharacterDataAsset->ActiveAbilities[Index]) return CharacterDataAsset->ActiveAbilities[Index].AbilityCost; }
	FORCEINLINE int32 GetFirstAbilityManaCost() const
	{ check(&CharacterDataAsset->ActiveAbilities[0]) return CharacterDataAsset->ActiveAbilities[0].AbilityCost; }
	FORCEINLINE int32 GetSecondAbilityManaCost() const
	{ check(&CharacterDataAsset->ActiveAbilities[1]) return CharacterDataAsset->ActiveAbilities[1].AbilityCost; }

	FORCEINLINE void DecreaseManaForFirstAbility() { DecreaseMana(GetFirstAbilityManaCost()); }
	FORCEINLINE void DecreaseManaForSecondAbility() { DecreaseMana(GetSecondAbilityManaCost()); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsInTheSameTeam(AKKCharacter* TargetCharacter) const { return TargetCharacter->OwningPlayer == OwningPlayer; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsCharacterOnMap() const { return OwnedTileID != -1; }
};