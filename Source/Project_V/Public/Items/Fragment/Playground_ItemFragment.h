// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Playground_ItemFragment.generated.h"

class APlayerController;

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

private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "FragmentTags"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;

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
struct FPlayground_ImageFragment : public FPlayground_ItemFragment
{
	GENERATED_BODY()

	UTexture2D* GetIcon() const { return Icon; }

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UTexture2D> Icon{ nullptr };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FVector2D IconDimensions{ 44.f, 44.f };
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

	virtual void OnConsume(APlayerController* PC) {}
};

USTRUCT(BlueprintType)
struct FPlayground_HealthPotionFragment : public FPlayground_ConsumableFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float HealAmount = 20.f;

	virtual void OnConsume(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FPlayground_ManaPotionFragment : public FPlayground_ConsumableFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float ManaAmount = 20.f;

	virtual void OnConsume(APlayerController* PC) override;
};