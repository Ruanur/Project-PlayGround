#pragma once

UENUM()
enum class EPlaygroundConfrimType : uint8
{
	Yes,
	No
};

UENUM()
enum class EPlaygroundValidType : uint8
{
	Valid,
	InValid
};

UENUM()
enum class EPlaygroundSuccessType : uint8
{
	Successful,
	Failed
};

//ÄðÅ¸ÀÓ Enum
UENUM()
enum class EPlaygroundCountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class EPlaygroundCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};

UENUM(BlueprintType)
enum class EPlaygroundGameDifficulty : uint8
{
	Easy,
	Normal,
	Hard,
	VeryHard
};

UENUM(BlueprintType)
enum class EPlaygroundInputMode : uint8
{
	GameOnly,
	UIOnly
};

UENUM(BlueprintType)
enum class EPlaygroundStatField : uint8
{
	CurrentHealth,
	MaxHealth,
	CurrentRage,
	MaxRage,
	AttackPower,
	DefensePower,
	BaseDamage
};