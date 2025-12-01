// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Playground_ItemFragment.generated.h"

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
	UPROPERTY(EditAnywhere, Category = "Inventory")
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
