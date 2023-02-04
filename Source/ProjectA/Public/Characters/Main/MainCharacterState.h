#pragma once

UENUM(BlueprintType)
enum class ECharacterState: uint8
{
	EAS_Unarmed UMETA(DisplayName = "Unarmed"),
	EAS_Armed UMETA(DisplayName = "Armed"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
	EAS_Attacking UMETA(DisplayName = "Attacking")
};