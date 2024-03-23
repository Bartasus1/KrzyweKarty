// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapStructs.h"
#include "GameFramework/Actor.h"
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
	
	bool AddCharacterToMap(AKKCharacter* Character, uint8 TileID); //todo: change those to void (later)
	bool MoveCharacter(AKKCharacter* Character, uint8 TileID);
	//bool MoveCharacter(AKKCharacter* Character, EMovementDirection MovementDirection);
	
	UFUNCTION(BlueprintCallable)
	TArray<AKKCharacter*> GetCharactersByDirection(AKKCharacter* Character, const TArray<FDirection>& Directions, ECharacterSelectionPolicy CharacterSelectionPolicy = CSP_AllCharacters, bool bBlockDirectionOnFound = false);

	UFUNCTION(BlueprintCallable)
	TArray<AKKTile*> GetTilesByDirection(AKKCharacter* Character, const TArray<FDirection>& Directions, ETileSelectionPolicy TileSelectionPolicy = TSP_AllTiles, bool bBlockDirectionOnFound = false);

	UFUNCTION(BlueprintCallable)
	TArray<FMapCell> GetCellsByDirection(AKKCharacter* Character, const TArray<FDirection>& Directions, bool bBlockDirectionOnFound = false);

	UFUNCTION(BlueprintCallable)
	TArray<AKKTile*> GetTilesForSpawn(AKKCharacter* Character, TArray<uint8> TilesID);

	UFUNCTION(BlueprintCallable)
	TArray<AKKTile*> GetTiles(TArray<uint8> TilesID);

	UFUNCTION(BlueprintCallable)
	TArray<AKKTile*> GetAllTilesOnMap();

	UFUNCTION(BlueprintCallable)
	TArray<AKKCharacter*> GetAllCharactersOnMap(AKKCharacter* CharacterToExclude = nullptr);

	UFUNCTION(BlueprintCallable)
	TArray<AKKCharacter*> GetAllyCharactersOnMap(AKKCharacter* Character);

	UFUNCTION(BlueprintCallable)
	TArray<AKKCharacter*> GetEnemyCharactersOnMap(AKKCharacter* Character);

	UFUNCTION(BlueprintCallable)
	bool AreCharactersInLine(AKKCharacter* Character, AKKCharacter* TargetCharacter) const;

	void GetTilesForBaseAttack(AKKCharacter* Character, TArray<AKKTile*>& InitialAttackTiles);
	
	bool CanAttackBase(const AKKCharacter* Character) const;
	
	UFUNCTION(BlueprintCallable)
	void ClearTilesHighlights();

	UFUNCTION(BlueprintCallable)
	AKKTile* GetTileAtIndex(uint8 TileID);

	UFUNCTION(BlueprintCallable)
	AKKCharacter* GetCharacterAtIndex(uint8 TileID);

	UFUNCTION(BlueprintCallable)
	AKKCharacter* GetCharacterByPosition(const FIntPoint& Position);

	void RemoveCharacterFromTile(uint8 TileID);

	void SpawnFraction(uint8 ID, TSubclassOf<AFraction> FractionClass);

	
	FMapCell* GetCellAtIndex(uint8 TileID);
	FMapCell* GetCellByPosition(const FIntPoint& Position);
	FMapCell* GetCellByDirection(const AKKCharacter* Character, const FDirection& Direction);
	
	FIntPoint GetPositionByTileID(int32 TileID) const;

protected:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* MapMesh;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<AKKTile> TileClass;
	

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void SetupMap();
	void SetupMapTiles();
	void SetupBaseTiles();
	
	void AssignCharacterToTile(AKKCharacter* Character, FMapCell* MapCell);
	void SetFractionBase(uint8 ID, AKKCharacter* Base);
	
	////   Map values   ////
	const FVector StartLocation = FVector(-250.f, -150.f, 0.1);
	const uint8 MapSize = 4;
	const uint8 TotalMapSize = 20;
	const uint8 BaseRow = 5;

	const uint8 FirstBaseAttackTiles[2] = {1, 2};
	const uint8 SecondBaseAttackTiles[2] = {17, 18};
	

	UPROPERTY(Replicated)
	TArray<FMapCell> BaseArray; //complimentary to MapArray

public:
	FORCEINLINE uint8 GetMapSize() const { return MapSize; }

private:
	FORCEINLINE int32 GetX(const uint8 TileID) const { return TileID / MapSize; }
	FORCEINLINE int32 GetY(const uint8 TileID) const { return TileID % MapSize; }
	FORCEINLINE int32 GetID(const uint8 X, const uint8 Y) const { return Y + (X * MapSize); }
	FORCEINLINE bool IsValidIndex(const uint8 X, const uint8 Y) { return MapArray.IsValidIndex(X) && MapArray[X].MapRows.IsValidIndex(Y); }
};
