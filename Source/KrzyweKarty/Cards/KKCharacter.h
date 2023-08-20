// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CharacterDataAsset.h"
#include "GameFramework/Actor.h"
#include "KrzyweKarty/Interfaces/SelectableInterface.h"
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
class KRZYWEKARTY_API AKKCharacter : public AActor, public ISelectableInterface, public IAbilitySystemInterface
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


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
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

	UPROPERTY(BlueprintReadOnly)
	UCharacterAttributeSet* CharacterAttributes;
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
	bool DefaultAttackConditions(AKKCharacter* TargetCharacter, EAttackType AttackType);
	bool MinAttackConditions(AKKCharacter* TargetCharacter, EAttackType AttackType);

	AKKMap* GetMap() const;

public:
	virtual int32 GetStrengthForAttack(AKKCharacter* TargetCharacter);
	virtual bool CanBeAttacked(EAttackType AttackType);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& LifetimeProps) const override;

	

public:
	FORCEINLINE FText GetCharacterName() const { check(CharacterDataAsset); return CharacterDataAsset->CharacterName; }
	
	FORCEINLINE int32 GetHealth()	const { return AbilitySystemComponent->GetSet<UCharacterAttributeSet>()->Health.GetCurrentValue(); }
	FORCEINLINE int32 GetMana()		const { return AbilitySystemComponent->GetSet<UCharacterAttributeSet>()->Mana.GetCurrentValue(); }
	FORCEINLINE int32 GetDefence()	const { return AbilitySystemComponent->GetSet<UCharacterAttributeSet>()->Defence.GetCurrentValue(); }
	FORCEINLINE int32 GetStrength() const { return AbilitySystemComponent->GetSet<UCharacterAttributeSet>()->Strength.GetCurrentValue(); }

	FORCEINLINE int32 GetDefaultHealth()   const { return AbilitySystemComponent->GetSet<UCharacterAttributeSet>()->Health.GetBaseValue(); }
	FORCEINLINE int32 GetDefaultMana()     const { return AbilitySystemComponent->GetSet<UCharacterAttributeSet>()->Mana.GetBaseValue(); }
	FORCEINLINE int32 GetDefaultDefence()  const { return AbilitySystemComponent->GetSet<UCharacterAttributeSet>()->Defence.GetBaseValue(); }
	FORCEINLINE int32 GetDefaultStrength() const { return AbilitySystemComponent->GetSet<UCharacterAttributeSet>()->Strength.GetBaseValue(); }

	FORCEINLINE void SetHealth(int32 NewHealth)		{ CharacterAttributes->Health.SetCurrentValue(FMath::Clamp(NewHealth, 0, GetDefaultHealth())); }
	FORCEINLINE void SetMana(int32 NewMana)			{ CharacterAttributes->Mana.SetCurrentValue(FMath::Clamp(NewMana, 0, GetDefaultHealth())); }
	FORCEINLINE void SetDefence(int32 NewDefence)	{ CharacterAttributes->Defence.SetCurrentValue(FMath::Clamp(NewDefence, 0, GetDefaultHealth())); }
	FORCEINLINE void SetStrength(int32 NewStrength) { CharacterAttributes->Strength.SetCurrentValue(FMath::Clamp(NewStrength, 0, GetDefaultHealth())); }

	FORCEINLINE void DecreaseHealth(int32 InHealth = 1)   { CharacterStats.Health -= FMath::Clamp(InHealth, 0, GetHealth()); }
	FORCEINLINE void DecreaseMana(int32 InMana = 1)		  { CharacterStats.Mana -= FMath::Clamp(InMana, 0, GetMana()); }
	FORCEINLINE void DecreaseDefence(int32 InDefence = 1) { CharacterStats.Defence -= FMath::Clamp(InDefence, 0, GetDefence()); }

	FORCEINLINE void IncreaseHealth(int32 InHealth = 1)		{ CharacterStats.Health += FMath::Clamp(InHealth, 0, GetDefaultHealth() - GetHealth()); }
	FORCEINLINE void IncreaseMana(int32 InMana = 1)			{ CharacterStats.Mana += FMath::Clamp(InMana, 0, GetDefaultMana() - GetMana()); }
	FORCEINLINE void IncreaseDefence(int32 InDefence = 1)	{ CharacterStats.Defence += FMath::Clamp(InDefence, 0, GetDefaultDefence() - GetDefence()); }

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




