// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayEffectTypes.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"

#include "Playground_ItemFragment.generated.h"

class APlayerController;
class UPlaygroundAbilitySystemComponent;
class UGameplayEffect;
class UPlayground_CompositeBase;
struct FGameplayAttribute;
struct FPlayground_ItemManifest;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayground_ItemFragment
{
	GENERATED_BODY()

	FPlayground_ItemFragment() {}

	FPlayground_ItemFragment(const FPlayground_ItemFragment&) = default;
	FPlayground_ItemFragment& operator = (const FPlayground_ItemFragment&) = default;
	FPlayground_ItemFragment(FPlayground_ItemFragment&&) = default;
	FPlayground_ItemFragment& operator = (FPlayground_ItemFragment&&) = default;
	virtual ~FPlayground_ItemFragment() {}

	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; }
	virtual void Manifest() {}

private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "FragmentTags"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;

};

/*
* Item fragment specifically for assimilation into a widget
*/
USTRUCT(BlueprintType)
struct FPlayground_InventoryItemFragment : public FPlayground_ItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UPlayground_CompositeBase* Composite) const;

protected:
	bool MatchesWidgetTag(const UPlayground_CompositeBase* Composite) const;
};

USTRUCT(BlueprintType)
struct FPlayground_GridFragment : public FPlayground_ItemFragment
{
	GENERATED_BODY()

	FIntPoint GetGridSize() const { return GridSize; }
	void SetGridSize(const FIntPoint& Size) { GridSize = Size; }
	float GetGridPadding() const { return GridPadding; }
	void SetGridPadding(float Padding) { GridPadding = Padding; }

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FIntPoint GridSize{ 1,1 };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float GridPadding{ 0.f };
};

USTRUCT(BlueprintType)
struct FPlayground_ImageFragment : public FPlayground_InventoryItemFragment
{
	GENERATED_BODY()

	UTexture2D* GetIcon() const { return Icon; }
	virtual void Assimilate(UPlayground_CompositeBase* Composite) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UTexture2D> Icon{ nullptr };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FVector2D IconDimensions{ 44.f, 44.f };
};

USTRUCT(BlueprintType)
struct FPlayground_TextFragment : public FPlayground_InventoryItemFragment
{
	GENERATED_BODY()

	FText GetText() const { return FragmentText; }
	void SetText(const FText& Text) { FragmentText = Text; }
	virtual void Assimilate(UPlayground_CompositeBase* Composite) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText FragmentText;
};

USTRUCT(BlueprintType)
struct FPlayground_LabeledNumberFragment : public FPlayground_InventoryItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UPlayground_CompositeBase* Composite) const override;
	virtual void Manifest() override;
	float GetValue() const { return Value; }
	void SetValue(float InValue) { Value = InValue; }

	// When manifesting for the first time, this fragment will randomized, However, one equipped
	// and dropped, an item should retain the same value, so randomization should not occur.
	bool bRandomizeOnManifest{ true };

private:
	 
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText Text_Label{};

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float Value{ 0.f };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Min{ 0 };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Max{ 0 };
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseLabel{ false };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseValue{ false };

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UGameplayEffect> SourceGameplayEffect;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MinFractionalDigits{ 1 };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxFractionalDigits{ 1 };
};

USTRUCT(BlueprintType)
struct FPlayground_StackableFragment : public FPlayground_ItemFragment
{
	GENERATED_BODY()

	int32 GetMaxStackSize() const { return MaxStackSize; }
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }

private:

	UPROPERTY(EditAnywhere, CateGory = "Inventory")
	int32 MaxStackSize{ 1 };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 StackCount{ 1 };
};

USTRUCT(BlueprintType)
struct FPlayground_ConsumableFragment : public FPlayground_ItemFragment
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC, UPlaygroundAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel) {}
};

USTRUCT(BlueprintType)
struct FPlayground_HealthPotionFragment : public FPlayground_ConsumableFragment
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> ConsumableGameplayEffectClass;

	virtual void OnConsume(APlayerController* PC, UPlaygroundAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel) override;
};

USTRUCT(BlueprintType)
struct FPlayground_ManaPotionFragment : public FPlayground_ConsumableFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float ManaAmount = 20.f;

	virtual void OnConsume(APlayerController* PC, UPlaygroundAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel) override;
};

// Equipment
//
USTRUCT(BlueprintType)
struct FPlayground_EquipModifier : public FPlayground_LabeledNumberFragment
{
	GENERATED_BODY()

	virtual void OnEquip(APlayerController* PC, float RarityMultiplier = 1.f) {}
	virtual void OnUnequip(APlayerController* PC) {}

};

USTRUCT(BlueprintType)
struct FPlayground_StrengthModifier : public FPlayground_EquipModifier
{
	GENERATED_BODY()

	// 장비 능력치 GameEffect
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EquipmentEffectClass;

	// 장착 중 적용된 GE Handle
	FActiveGameplayEffectHandle ActiveEffectHandle;

	virtual void OnEquip(APlayerController* PC, float RarityMultiplier) override;
	virtual void OnUnequip(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FPlayground_BaseDamageModifier : public FPlayground_EquipModifier
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EquipmentEffectClass;

	// 장착 중 적용된 GE Handle
	FActiveGameplayEffectHandle ActiveEffectHandle;

	virtual void OnEquip(APlayerController* PC, float RarityMultiplier) override;
	virtual void OnUnequip(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FPlayground_HealthModifier : public FPlayground_EquipModifier
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EquipmentEffectClass;

	// 장착 중 적용된 GE Handle
	FActiveGameplayEffectHandle ActiveEffectHandle;

	virtual void OnEquip(APlayerController* PC, float RarityMultiplier) override;
	virtual void OnUnequip(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FPlayground_DefenseModifier : public FPlayground_EquipModifier
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EquipmentEffectClass;

	// 장착 중 적용된 GE Handle
	FActiveGameplayEffectHandle ActiveEffectHandle;

	virtual void OnEquip(APlayerController* PC, float RarityMultiplier) override;
	virtual void OnUnequip(APlayerController* PC) override;
};

class APlayground_EquipActor;
USTRUCT(BlueprintType)
struct FPlayground_EquipmentFragment : public FPlayground_InventoryItemFragment
{
	GENERATED_BODY()

	bool bEquipped{ false };
	void OnEquip(APlayerController* PC, float RarityMultiplier = 1.f);
	void OnUnequip(APlayerController* PC);
	virtual void Assimilate(UPlayground_CompositeBase* Composite) const override;
	virtual void Manifest() override;

	FPlayground_StrengthModifier* GetStrengthModifierMutable()
	{
		for (auto& Modifier : EquipModifiers)
		{
			if (FPlayground_StrengthModifier* StrengthModifier = Modifier.GetMutablePtr<FPlayground_StrengthModifier>())
			{
				return StrengthModifier;
			}
		}

		return nullptr;
	}

	FPlayground_BaseDamageModifier* GetBaseDamageModifierMutable()
	{
		for (auto& Modifier : EquipModifiers)
		{
			if (FPlayground_BaseDamageModifier* BaseDamageModifier = Modifier.GetMutablePtr<FPlayground_BaseDamageModifier>())
			{
				return BaseDamageModifier;
			}
		}

		return nullptr;
	}

	FPlayground_HealthModifier* GetHealthModifierMutable()
	{
		for (auto& Modifier : EquipModifiers)
		{
			if (FPlayground_HealthModifier* HealthModifier = Modifier.GetMutablePtr<FPlayground_HealthModifier>())
			{
				return HealthModifier;
			}
		}

		return nullptr;
	}

	FPlayground_DefenseModifier* GetDefenseModifierMutable()
	{
		for (auto& Modifier : EquipModifiers)
		{
			if (FPlayground_DefenseModifier* DefenseModifier = Modifier.GetMutablePtr<FPlayground_DefenseModifier>())
			{
				return DefenseModifier;
			}
		}

		return nullptr;
	}

	APlayground_EquipActor* SpawnAttachedActor(USkeletalMeshComponent* AttachMesh) const;
	void DestroyAttachedActor() const;
	FGameplayTag GetEquipmentType() const { return EquipmentType; }
	void SetEquippedActor(APlayground_EquipActor* EquipActor);

private:

	//meta = (ExcludeBaseStruct) 사용 시 반드시 StructUtils/InstancedStruct.h Include 하기
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<TInstancedStruct<FPlayground_EquipModifier>> EquipModifiers;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<APlayground_EquipActor> EquipActorClass = nullptr;

	TWeakObjectPtr<APlayground_EquipActor> EquippedActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FName SocketAttachPoint{NAME_None};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag EquipmentType = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FPlayground_ItemRarity : public FPlayground_InventoryItemFragment
{
	GENERATED_BODY()

public:
	EPlaygroundRarity GetRarity() const { return Rarity; }
	void SetRarity(EPlaygroundRarity InRarity) { Rarity = InRarity; }
	virtual void Assimilate(UPlayground_CompositeBase* Composite) const override;

	float GetStatMultiplier() const
	{
		switch (Rarity)
		{
		case EPlaygroundRarity::Common:
			return 1.0f;
		case EPlaygroundRarity::Uncommon:
			return 1.5f;;
		case EPlaygroundRarity::Rare:
			return 1.8f;
		case EPlaygroundRarity::Epic:
			return 2.0f;
		case EPlaygroundRarity::Legendary:
			return 2.5f;
		default:
			return 1.0f;
		}
	}

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	EPlaygroundRarity Rarity = EPlaygroundRarity::Common;

};