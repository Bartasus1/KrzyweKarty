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

UENUM(BlueprintType)
enum EMovementType
{
	EMP_SummonedCharacter	UMETA(DisplayName="Summoned Character"),
	EMP_MovedCharacter		UMETA(DisplayName="Moved Character"),
	EMP_AttackCharacter		UMETA(DisplayName="Attacked Character")
};

UENUM()
enum EAttackResult
{
	AttackDenied, //No Mana
	AttackBlocked, //Blocked by another character 
	AttackConfirmed // Successful attack
};

USTRUCT(BlueprintType)
struct FAttackResult
{
	GENERATED_BODY()
	
	FAttackResult() {}
	FAttackResult(EAttackResult InAttackResult, FText Text): AttackResultEnum(InAttackResult), AttackResultText(Text) { }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EAttackResult> AttackResultEnum = AttackConfirmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AttackResultText;
	
};

UCLASS(BlueprintType)
class UKKDamage : public UObject
{
	GENERATED_BODY()

public:

	UKKDamage() {}
	UKKDamage(EAttackType InAttackType) : AttackType(InAttackType)
	{
		
	}

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EAttackType> AttackType = EAT_DefaultAttack;
};




UCLASS(BlueprintType)
class UDefaultAttackDamage : public UKKDamage
{
	GENERATED_BODY()

public:
	UDefaultAttackDamage() : UKKDamage(EAT_DefaultAttack)
	{
		
	}
};

UCLASS(BlueprintType)
class UActiveAbilityDamage : public UKKDamage
{
	GENERATED_BODY()

public:
	UActiveAbilityDamage() : UKKDamage(EAT_ActiveAbility)
	{
		
	}
};


