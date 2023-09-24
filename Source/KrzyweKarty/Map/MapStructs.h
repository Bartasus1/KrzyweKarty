#pragma once

#include "CoreMinimal.h"
#include "MapStructs.generated.h"

class AKKCharacter;
class AKKTile;

USTRUCT(BlueprintType)
struct FMapCell
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AKKTile* Tile;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AKKCharacter* Character;
};

USTRUCT(BlueprintType)
struct FMapRow
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FMapCell> MapRows;
};

UENUM()
enum ERotationDirection
{
	ERD_Forward = 1	UMETA(DisplayName = "Forward"),
	ERD_Right		UMETA(DisplayName = "Right"),
	ERD_Backward	UMETA(DisplayName = "Backward"),
	ERD_Left		UMETA(DisplayName = "Left")
};

USTRUCT(BlueprintType)
struct FDirection
{
	GENERATED_BODY()

	FDirection(): X(0), Y(0) {}
	FDirection(int32 InX, int32 InY): X(InX), Y(InY) {}
	FDirection(const FDirection &Other): X(Other.X), Y(Other.Y) {}
	

	UPROPERTY(BlueprintReadWrite)
	int32 X; // vertical

	UPROPERTY(BlueprintReadWrite)
	int32 Y; // horizontal
	
	FDirection Rotate(const ERotationDirection Rotation) const;
};

UENUM(BlueprintType)
enum ETileSelectionPolicy
{
	TSP_AllTiles				UMETA(DisplayName="None"),
	TSP_NoCharacters		UMETA(DisplayName="No Characters"),
	TSP_AllyCharactersOnly	UMETA(DisplayName="Ally Characters Only"),
	TSP_EnemyCharactersOnly UMETA(DisplayName="Enemy Characters Only")
};

UENUM(BlueprintType)
enum ECharacterSelectionPolicy
{
	CSP_AllCharacters		UMETA(DisplayName="All Characters"),
	CSP_AllyCharactersOnly	UMETA(DisplayName="Ally Characters Only"),
	CSP_EnemyCharactersOnly UMETA(DisplayName="Enemy Characters Only")
};