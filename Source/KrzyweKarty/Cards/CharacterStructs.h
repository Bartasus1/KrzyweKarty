#pragma once
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
	int32 AbilityCost;

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

UENUM(BlueprintType)
enum EMovementType
{
	EMP_SummonedCharacter	UMETA(DisplayName="Summoned Character"),
	EMP_MovedCharacter		UMETA(DisplayName="Moved Character"),
	EMP_AttackCharacter		UMETA(DisplayName="Attacked Character")
};

UENUM()
enum class EAttackResult
{
	AttackDenied, //No attack conditions met
	AttackBlocked, //Blocked by another character 
	AttackConfirmed // Successful attack
};

USTRUCT(BlueprintType)
struct FAttackResultInfo
{
	GENERATED_BODY()
	
	FAttackResultInfo() {}
	FAttackResultInfo(EAttackResult InAttackResult, FText Text): AttackResultEnum(InAttackResult), AttackResultText(Text) { }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackResult AttackResultEnum = EAttackResult::AttackConfirmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AttackResultText;
	
};