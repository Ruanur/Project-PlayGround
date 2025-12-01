// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Types/Playground_GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"

#include "Playground_ItemManifest.generated.h"

/**
 * 
 */
class UPlayground_InventoryItem;
struct FPlayground_ItemFragment;

USTRUCT()
struct PROJECT_V_API FPlayground_ItemManifest 
{
	GENERATED_BODY()

	UPlayground_InventoryItem* Manifest(UObject* NewOuter);
	EPlayground_ItemCategory GetItemCategory() const { return ItemCategory; }
	FGameplayTag GetItemType() const { return ItemType; }

	template<typename T> requires std::derived_from<T, FPlayground_ItemFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FPlayground_ItemFragment>> Fragments;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	EPlayground_ItemCategory ItemCategory{ EPlayground_ItemCategory::None };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag ItemType;
};

template<typename T> requires std::derived_from<T, FPlayground_ItemFragment>
const T* FPlayground_ItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const
{
	for (const TInstancedStruct<FPlayground_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag)) continue;
			return FragmentPtr;
		}
	}

	return nullptr;
}