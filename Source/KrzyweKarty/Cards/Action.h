// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStructs.h"
#include "UObject/Object.h"
#include "Action.generated.h"

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

	UFUNCTION(BlueprintAuthorityOnly)
	void AddActionToCharacterList() const;
	
	AKKGameState* GetGameState() const;
	AKKMap* GetMap() const;
};

/////////////////////////////////////////////////////////////////////////////////////
UCLASS(Abstract)
class UMapAction : public UAction
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	int32 DestinationTileID = -1;

	virtual bool CanCharacterMakeAction() const override;
};
//////////////////////////////////////////////////////////////////////////////////

UCLASS()
class USummonAction : public UMapAction
{
	GENERATED_BODY()

public:

	USummonAction();

	virtual bool CanCharacterMakeAction() const override;
	virtual void BeginAction() override;

	virtual void ShowActionAffectedTiles() const override;
};
/////////////////////////////////////////////////////////////////////////////
UCLASS()
class UMoveAction : public UMapAction
{
	GENERATED_BODY()

public:

	UMoveAction();

	virtual bool CanCharacterMakeAction() const override;
	virtual void BeginAction() override;

	virtual void ShowActionAffectedTiles() const override;
};

///////////////////////////////////////////////////////////////////////////////////

UCLASS()
class UAttackAction : public UAction
{
	GENERATED_BODY()

public:

	UAttackAction();
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	AKKCharacter* TargetCharacter = nullptr;

	virtual void TryBeginAction() override;
	virtual void BeginAction() override;

	virtual void ShowActionAffectedTiles() const override;
};
