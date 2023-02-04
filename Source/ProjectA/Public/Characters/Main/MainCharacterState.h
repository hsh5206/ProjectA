#pragma once

UENUM(BlueprintType)
enum class ECharacterState: uint8
{
	EAS_Unarmed UMETA(DisplayName = "Unarmed"),
	EAS_Armed UMETA(DisplayName = "Armed"),
	EAS_UnarmedToLockOn UMETA(DisplayName = "UnarmedToLockOn"),
	EAS_LockOn UMETA(Displayname = "LockOn")
};