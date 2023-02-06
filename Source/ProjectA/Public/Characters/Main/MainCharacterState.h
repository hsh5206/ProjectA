#pragma once

UENUM(BlueprintType)
enum class ECharacterArmedState: uint8
{
	EAS_Unarmed UMETA(DisplayName = "Unarmed"),
	EAS_Armed UMETA(DisplayName = "Armed"),
	EAS_UnarmedToLockOn UMETA(DisplayName = "UnarmedToLockOn"),
	EAS_LockOn UMETA(Displayname = "LockOn")
};

UENUM(BlueprintType)
enum class ECharacterCombatState : uint8
{
	ECS_Default UMETA(DisplayName = "Default"),
	EAS_Jumping UMETA(DisplayName = "Jumping"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Rolling UMETA(DisplayName = "Rolling")
};

struct FCharacterRoll

{
public:
	bool bFront = true;
	bool bBack = false;
	bool bRight = false;
	bool bLeft = false;
};