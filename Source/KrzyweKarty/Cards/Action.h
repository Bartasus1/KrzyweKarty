﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStructs.h"
#include "UObject/Object.h"
#include "Action.generated.h"

class UCharacterAbilityComponent;
class AKKMap;
class AKKGameState;
/**
 * 
 */
UCLASS(Blueprintable, Blueprintable, Abstract)
class UAction : public UObject
{
	GENERATED_BODY()
public:
	UAction();

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	AKKCharacter* Character = nullptr;

	UFUNCTION(BlueprintCallable)
	virtual void TryBeginAction();

	virtual bool CanCharacterMakeAction() const;
	virtual void BeginAction();

	UFUNCTION(BlueprintCallable)
	virtual void ShowActionAffectedTiles() const;
	
	int32 GetActionWeight() const;

protected:
	int32 ActionWeight = 0;
	bool bRequiresCharacterOnMap = false;

	UFUNCTION(BlueprintAuthorityOnly)
	void AddActionToCharacterList() const;
	
	AKKGameState* GetGameState() const;
	AKKMap* GetMap() const;
};

/////////////////////////////////////////////////////////////////////////////////////
/// SUMMON ACTION
UCLASS()
class USummonAction : public UAction
{
	GENERATED_BODY()

public:
	USummonAction();

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	int32 DestinationTileID = -1;

	virtual bool CanCharacterMakeAction() const override;
	virtual void BeginAction() override;

	virtual void ShowActionAffectedTiles() const override;
};

/////////////////////////////////////////////////////////////////////////////
/// MOVE ACTION
UCLASS()
class UMoveAction : public UAction
{
	GENERATED_BODY()

public:
	UMoveAction();

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	int32 DestinationTileID = -1;
	
	virtual bool CanCharacterMakeAction() const override;
	virtual void BeginAction() override;

	virtual void ShowActionAffectedTiles() const override;
};

///////////////////////////////////////////////////////////////////////////////////
/// ATTACK ACTION
UCLASS()
class UAttackAction : public UAction
{
	GENERATED_BODY()

public:

	UAttackAction();
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	AKKCharacter* TargetCharacter = nullptr;
	
	virtual bool CanCharacterMakeAction() const override;
	virtual void BeginAction() override;

	virtual void ShowActionAffectedTiles() const override;
};

//////////////////////////////////////////////////////////////////////////////////
/// ABILITY ACTION
UCLASS()
class UAbilityAction : public UAction
{
	GENERATED_BODY()

public:

	UAbilityAction();

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	int32 Index = 0;

	UFUNCTION(BlueprintCallable)
	void OnAbilityConfirmed();

	UFUNCTION(BlueprintCallable)
	void OnAbilityAborted();

	virtual void TryBeginAction() override;
	
	virtual bool CanCharacterMakeAction() const override;
	virtual void BeginAction() override;
	
};