// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDataAsset.h"
#include "GameFramework/Actor.h"
#include "KrzyweKarty/Interfaces/SelectableInterface.h"
#include "KKCharacter.generated.h"

class AKKGameState;
class UAction;
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterStatsDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterAbilityAction, uint8, Index);


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
	uint8 CharacterID = 0;
	
	UPROPERTY(BlueprintAssignable)
	FCharacterDiedDelegate OnCharacterDeath;

	UPROPERTY(BlueprintAssignable)
	FCharacterStatsDelegate OnCharacterStatsChanged;
	
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	uint8 CharacterActions;

	UPROPERTY(Replicated, BlueprintReadOnly)
	AAbilityActor* AbilityActor;
protected:
	
	UPROPERTY(ReplicatedUsing="OnRep_CharacterStats", BlueprintReadWrite, VisibleAnywhere) // track stats in game
	FCharacterStats CharacterStats;

	UFUNCTION()
	void OnRep_CharacterStats() const;

public:
///////////////////////////////////////////////////////////////////////	
	// Interaction with the Map
	UFUNCTION(BlueprintCallable)
	virtual TArray<uint8> GetPossibleSpawnTiles();
	
	UFUNCTION(BlueprintCallable)
	virtual TArray<FDirection> GetPossibleMoveTiles();

	UFUNCTION(BlueprintCallable)
	virtual TArray<FDirection> GetPossibleAttackTiles();

/////////////////////////////////////////////////////////////
	// Actions & Rounds
	uint8 GetTopActionWeight();
	
	UFUNCTION()
	virtual void OnTurnEnded();
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void SetOwner(AActor* NewOwner) override;

///////////////////////////////////////////////////////////////////////
	//Attacking Actions

	virtual FAttackResultInfo DefaultAttack(AKKCharacter* TargetCharacter);
	
	virtual int32 DefineDamageAmount(AKKCharacter* TargetCharacter);
	virtual void ApplyDamageToSelf(int32 DamageAmount, FAttackResultInfo& AttackResultInfo, AKKCharacter* DamageCauser);

	UFUNCTION(BlueprintCallable)
	FCharacterStats CalculateCharacterStatsAfterAttack(AKKCharacter* TargetCharacter);
	
//////////////////////////////////////////////////////////////////////////////////////////////
	// Abilities Actions

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanUseAbility(uint8 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanFinishAbility(uint8 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PerformAbility(uint8 Index); // actual implementation of ability, how many damage it deals etc.

	UFUNCTION(BlueprintNativeEvent)
	void OnBeginAbility(uint8 Index); //start ability components, show ability effects etc.

	UFUNCTION(BlueprintNativeEvent)
	void OnFinishAbility(uint8 Index); //stop ability components, clean up effects etc.
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CommitAbilityCost(uint8 Index);

	UPROPERTY(BlueprintAssignable)
	FCharacterAbilityAction OnBeginAbilityDelegate;

	UPROPERTY(BlueprintAssignable)
	FCharacterAbilityAction OnFinishAbilityDelegate;

	UFUNCTION(Client, Reliable)
	void Client_OnBeginAbilityDelegate(uint8 Index);

	UFUNCTION(Client, Reliable)
	void Client_OnFinishAbilityDelegate(uint8 Index);

	UFUNCTION(BlueprintPure)
	TSubclassOf<AAbilityActor> GetAbilityClass(uint8 Index) const;

	//////////////////////////////////////////////////////
	// Tiles

	UFUNCTION(BlueprintCallable)
	virtual uint8 GetTilePositionID() const override;

	virtual void OnSelectableHighlighted() override;

	virtual void OnSelectableGainFocus() override;
	virtual void OnSelectableLostFocus() override;

	UFUNCTION(NetMulticast, Reliable)
	void SetCollisionResponseToChannel(ECollisionChannel CollisionChannel, ECollisionResponse CollisionResponse);

	UFUNCTION(BlueprintCallable)
	virtual TArray<AKKTile*> GetMoveTiles();


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

	int32 GetDistanceTo(const TScriptInterface<ISelectableInterface>& SelectableInterface) const;

	UFUNCTION(BlueprintPure)
	bool IsFirstCharacterInLine(AKKCharacter* TargetCharacter);

////////////////////////////////////////////////////////////////

	AKKGameState* GetGameState() const;
	AKKMap* GetMap() const;

////////////////////////////////////////////////////////////////
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& LifetimeProps) const override;

public:
	FORCEINLINE FText GetCharacterName() const { return FText::Join(FText::FromString(""), CharacterDataAsset->CharacterName, FText::FromString("(") ,FText::AsNumber(CharacterID),FText::FromString(")")); }

	FORCEINLINE const FCharacterStats& GetCharacterStats() const { return  CharacterStats; }
	
	FORCEINLINE int32 GetCharacterStatistic(int32 FCharacterStats::* MemberField) const { return CharacterStats.*MemberField; }
	FORCEINLINE int32 GetCharacterDefaultStatistic(int32 FCharacterStats::* MemberField) const { return CharacterDataAsset->CharacterStats.*MemberField; }
	
	FORCEINLINE int32 GetHealth()	const { return CharacterStats.Health; }
	FORCEINLINE int32 GetMana()		const { return CharacterStats.Mana; }
	FORCEINLINE int32 GetDefence()	const { return CharacterStats.Defence; }
	FORCEINLINE int32 GetStrength() const { return CharacterStats.Strength; }

	FORCEINLINE int32 GetDefaultHealth()   const { return CharacterDataAsset->CharacterStats.Health; }
	FORCEINLINE int32 GetDefaultMana()     const { return CharacterDataAsset->CharacterStats.Mana; }
	FORCEINLINE int32 GetDefaultDefence()  const { return CharacterDataAsset->CharacterStats.Defence; }
	FORCEINLINE int32 GetDefaultStrength() const { return CharacterDataAsset->CharacterStats.Strength; }

	FORCEINLINE void SetHealth(int32 NewHealth) 	{ CharacterStats.Health		=	FMath::Clamp<int32>(NewHealth, 0, GetDefaultHealth());		OnRep_CharacterStats(); }
	FORCEINLINE void SetMana(int32 NewMana)			{ CharacterStats.Mana		=	FMath::Clamp<int32>(NewMana, 0, GetDefaultMana());			OnRep_CharacterStats(); }
	FORCEINLINE void SetDefence(int32 NewDefence)	{ CharacterStats.Defence	=	FMath::Clamp<int32>(NewDefence, 0, GetDefaultDefence());		OnRep_CharacterStats(); }
	FORCEINLINE void SetStrength(int32 NewStrength) { CharacterStats.Strength	=	FMath::Clamp<int32>(NewStrength, 0, GetDefaultStrength());	OnRep_CharacterStats(); }

	FORCEINLINE void DecreaseHealth(int32 InHealth = 1)   { SetHealth(GetHealth() - InHealth); }
	FORCEINLINE void DecreaseMana(int32 InMana = 1)		  { SetMana(GetMana() - InMana); }
	FORCEINLINE void DecreaseDefence(int32 InDefence = 1) { SetDefence(GetDefence() - InDefence); }

	FORCEINLINE void IncreaseHealth(int32 InHealth = 1)		 { SetHealth(GetHealth() + InHealth); }
	FORCEINLINE void IncreaseMana(int32 InMana = 1)			 { SetMana(GetMana() + InMana); }
	FORCEINLINE void IncreaseDefence(int32 InDefence = 1)	 { SetDefence(GetDefence() + InDefence); }

	FORCEINLINE int32 GetActiveAbilityCost(uint8 Index) const
	{ return CharacterDataAsset->ActiveAbilities[Index].AbilityCost; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsInTheSameTeam(AKKCharacter* TargetCharacter) const { return TargetCharacter->Direction == Direction; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsCharacterOnMap() const { return OwnedTileID != -1; }
};

