// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapStructs.h"
#include "Components/ArrowComponent.h"
#include "KKMap.generated.h"

class AFraction;
class UStaticMeshComponent;
class AKKCharacter;
class AKKTile;





UCLASS()
class KRZYWEKARTY_API AKKMap : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKKMap();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	TArray<FMapRow> MapArray;
	
	bool AddCharacterToMap(AKKCharacter* Character, int32 TileID); //todo: change those to void (later)
	bool MoveCharacter(AKKCharacter* Character, int32 TileID);
	//bool MoveCharacter(AKKCharacter* Character, EMovementDirection MovementDirection);
	
	UFUNCTION(BlueprintCallable)
	TArray<AKKCharacter*> GetCharactersByDirection(AKKCharacter* Character, TArray<FDirection> RelativeTiles, ECharacterSelectionPolicy CharacterSelectionPolicy = CSP_AllCharacters);

	UFUNCTION(BlueprintCallable)
	TArray<AKKTile*> GetTilesByDirection(AKKCharacter* Character, TArray<FDirection> RelativeTiles, ETileSelectionPolicy TileSelectionPolicy = TSP_AllTiles);

	UFUNCTION(BlueprintCallable)
	TArray<FMapCell> GetCellsByDirection(AKKCharacter* Character, TArray<FDirection> RelativeTiles);

	UFUNCTION(BlueprintCallable)
	TArray<AKKTile*> GetTilesForSpawn(AKKCharacter* Character, TArray<int32> TilesID);

	UFUNCTION(BlueprintCallable)
	TArray<AKKTile*> GetTiles(TArray<int32> TilesID);

	UFUNCTION(BlueprintCallable)
	TArray<AKKTile*> GetAllTilesOnMap();

	UFUNCTION(BlueprintCallable)
	TArray<AKKCharacter*> GetAllCharactersOnMap();

	UFUNCTION(BlueprintCallable)
	TArray<AKKCharacter*> GetAllyCharactersOnMap(AKKCharacter* Character);

	UFUNCTION(BlueprintCallable)
	TArray<AKKCharacter*> GetEnemyCharactersOnMap(AKKCharacter* Character);

	TArray<AKKTile*> CanAttackBase(AKKCharacter* Character, TArray<AKKTile*> InDefaultAttackTiles);
	
	UFUNCTION(BlueprintCallable)
	void ClearTilesHighlights();

	UFUNCTION(BlueprintCallable)
	AKKTile* GetTileAtIndex(int32 TileID);

	UFUNCTION(BlueprintCallable)
	AKKCharacter* GetCharacterAtIndex(int32 TileID);

	void RemoveCharacterFromTile(int32 TileID);

	
	FMapCell* GetCellAtIndex(int32 TileID);
	FMapCell* GetCellByDirection(AKKCharacter* Character, FDirection Direction);

protected:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* MapMesh;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AKKTile> TileClass;
	

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void SetupMap();
	void AssignCharacterToTile(AKKCharacter* Character, FMapCell* MapCell);
	void SetFractionBase(int32 ID, AKKCharacter* Base);
	
	friend class AKKSpawnpoint;
	
	////   Map values   ////
	const FVector StartLocation = FVector(-250.f, -150.f, 0.1);
	const uint8 MapSize = 4;
	const uint8 TotalMapSize = 20;
	const uint8 BaseRow = 5;

	UPROPERTY(Replicated)
	TArray<FMapCell> BaseArray;

public:
	FORCEINLINE uint8 GetMapSize() const { return MapSize; }

private:
	FORCEINLINE int32 GetX(int32 TileID) const { return TileID / MapSize; }
	FORCEINLINE int32 GetY(int32 TileID) const { return TileID % MapSize; }
	FORCEINLINE int32 GetID(int32 X, int32 Y) const { return Y + (X * MapSize); }
	FORCEINLINE bool IsValidIndex(int32 X, int32 Y) { return MapArray.IsValidIndex(X) && MapArray[X].MapRows.IsValidIndex(Y); }
};
