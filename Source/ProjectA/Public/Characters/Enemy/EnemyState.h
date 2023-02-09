#pragma once

UENUM(BlueprintType)
enum class EEnemyState: uint8
{
	EES_Default UMETA(DisplayName = "Default"),
	EES_Moving UMETA(DisplayName = "Moving"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Dead UMETA(DisplayName = "Dead")
};