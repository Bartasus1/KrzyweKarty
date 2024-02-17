// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAbilityComponent.h"
#include "KrzyweKarty/Map/KKTile.h"
#include "KrzyweKarty/Map/MapStructs.h"
#include "AreaEffectCharacterAbilityComponent.generated.h"


struct FInputActionInstance;

UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
class KRZYWEKARTY_API UAreaEffectCharacterAbilityComponent : public UCharacterAbilityComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAreaEffectCharacterAbilityComponent();

	virtual void OnBeginAbility_Implementation(int32 Index) override;
	virtual void OnFinishAbility_Implementation(int32 Index) override;

	UFUNCTION(Server, Reliable)
	void Server_SetAffectedTiles(const TArray<FDirection>& InAffectedTiles);
	
	UFUNCTION(BlueprintCallable)
	const TArray<FDirection>& GetFinalAffectedTiles() const;
	
	UFUNCTION(BlueprintCallable)
	void RotateSelectedTiles(const FInputActionInstance& InputAction);

protected:

	UPROPERTY(Replicated)
	TArray<FDirection> AffectedTiles;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETileSelectionPolicy> TileSelectionPolicy = TSP_AllTiles;

	UPROPERTY(EditAnywhere)
	ETileColor TileColor = ETileColor::Red;

private:
	UFUNCTION(Client, Reliable)
	void ShowAreaTiles();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
