// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDataAsset.h"
#include "GameFramework/Actor.h"
#include "KrzyweKarty/Components/CharacterAbilityComponent.h"
#include "KrzyweKarty/Interfaces/SelectableInterface.h"
#include "KrzyweKarty/Map/KKMap.h"
#include "KKCharacter.generated.h"

class UAction;
struct FMapCell;
enum ERotationDirection : int;
class AKKTile;
class AKKMap;
struct FDirection;
class UCharacterDataAsset;
class AKKPlayerController;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UTextRenderComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterDiedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterAbilityAction, int32, Index);


UCLASS(Abstract)
class KRZYWEKARTY_API AKKCharacter : public AActor, public ISelectableInterface
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
	TArray<int32> CharacterActions;
protected:
	
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere) // track stats in game
	FCharacterStats CharacterStats;

public:
///////////////////////////////////////////////////////////////////////	
	// Interaction with the Map
	UFUNCTION(BlueprintCallable)
	virtual TArray<int32> GetPossibleSpawnTiles();
	
	UFUNCTION(BlueprintCallable)
	virtual TArray<FDirection> GetPossibleMoveTiles();

	UFUNCTION(BlueprintCallable)
	virtual TArray<FDirection> GetPossibleAttackTiles();

/////////////////////////////////////////////////////////////
	// Actions & Rounds
	int32 GetTopActionWeight();
	
	UFUNCTION()
	virtual void OnTurnEnded();
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
public:
	virtual void SetOwner(AActor* NewOwner) override;

///////////////////////////////////////////////////////////////////////
	//Attacking Actions

	virtual FAttackResultInfo DefaultAttack(AKKCharacter* TargetCharacter);
	
	virtual int32 DefineDamageAmount(AKKCharacter* TargetCharacter);
	virtual void ApplyDamageToSelf(int32 DamageAmount, FAttackResultInfo& AttackResultInfo);
	
//////////////////////////////////////////////////////////////////////////////////////////////
	// Abilities Actions

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanUseAbility(int32 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanFinishAbility(int32 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PerformAbility(int32 Index); // actual implementation of ability, how many damage it deals etc.

	UFUNCTION(BlueprintNativeEvent)
	void OnBeginAbility(int32 Index); //start ability components, show ability effects etc.

	UFUNCTION(BlueprintNativeEvent)
	void OnFinishAbility(int32 Index); //stop ability components, clean up effects etc.
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CommitAbilityCost(int32 Index);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UCharacterAbilityComponent* GetCharacterAbilityComponent(int32 Index);

	template<typename T>
	T* GetCharacterAbilityComponent(int32 Index);

	UPROPERTY(BlueprintAssignable)
	FCharacterAbilityAction OnBeginAbilityDelegate;

	UPROPERTY(BlueprintAssignable)
	FCharacterAbilityAction OnFinishAbilityDelegate;

	UFUNCTION(Client, Reliable)
	void Client_OnBeginAbilityDelegate(int32 Index);

	UFUNCTION(Client, Reliable)
	void Client_OnFinishAbilityDelegate(int32 Index);
	
	// UFUNCTION(BlueprintCallable)
	// virtual void ActiveAbility(int32 Index, TScriptInterface<ISelectableInterface> SelectableObject);
	

	UFUNCTION(BlueprintCallable)
	virtual int32 GetTilePositionID() override;

	UFUNCTION(BlueprintCallable)
	virtual TArray<AKKTile*> GetMoveTiles();

	UFUNCTION(BlueprintCallable)
	virtual TArray<AKKTile*> GetAttackTiles();


/////////////////////////////////////////////////////////
	// Networked functions
	
	UFUNCTION(NetMulticast, Unreliable)
	void PlayAnimMontage(UAnimMontage* AnimMontage);

	UFUNCTION(NetMulticast, Reliable)
	void CharacterDied();

protected:
////////////////////////////////////////////////////////////////	
	//Subclass Sandbox
	void KillCharacter(AKKCharacter* TargetCharacter) const;
	void DealDamage(AKKCharacter* TargetCharacter, int32 Damage) const;

	int32 GetDistanceTo(AKKCharacter* TargetCharacter) const;

////////////////////////////////////////////////////////////////

	AKKGameState* GetGameState() const;
	AKKMap* GetMap() const;

////////////////////////////////////////////////////////////////
	virtual void BeginPlay() override;
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
	{ return CharacterDataAsset->ActiveAbilities[Index].AbilityCost; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsInTheSameTeam(AKKCharacter* TargetCharacter) const { return TargetCharacter->OwningPlayer == OwningPlayer; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsCharacterOnMap() const { return OwnedTileID != -1; }
};

template <typename T>
T* AKKCharacter::GetCharacterAbilityComponent(int32 Index)
{
	return Cast<T>(GetCharacterAbilityComponent(Index));
}
