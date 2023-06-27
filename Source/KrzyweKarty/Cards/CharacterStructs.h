#pragma once
#include "Engine/DataTable.h"
#include "EnhancedInput/Public/InputMappingContext.h"

#include "CharacterStructs.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Defence;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Mana;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAttackRange = 1;
};

USTRUCT(BlueprintType)
struct FAbilityDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AbilityDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AbilityImage;
	
};

UENUM()
enum EAttackType
{
	EAT_DefaultAttack,
	EAT_ActiveAbility,
	EAT_PassiveAbility
};

USTRUCT()
struct FCharacterAction : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* MappingContext;
	
};


USTRUCT(BlueprintType)
struct FCharacterActionsBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKey ActionKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ActionNameText;

	friend bool operator==(const FCharacterActionsBase& First, const FCharacterActionsBase& Second)
	{
		return First.ActionKey.GetFName() == Second.ActionKey.GetFName();
	}
};

static inline FCharacterActionsBase SpawnAction = {EKeys::N, FText::FromString("Add to Map")};
static inline FCharacterActionsBase AttackAction = {EKeys::X, FText::FromString("Attack")};
static inline FCharacterActionsBase FirstAbilityAction = {EKeys::J, FText::FromString("First Ability")};
static inline FCharacterActionsBase MoveAction = {EKeys::M, FText::FromString("Move")};