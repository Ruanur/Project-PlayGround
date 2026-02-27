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
class UPlayground_CompositeBase;
struct FPlayground_ItemFragment;

USTRUCT()
struct PROJECT_V_API FPlayground_ItemManifest 
{
	GENERATED_BODY()

	TArray<TInstancedStruct<FPlayground_ItemFragment>>& GetFragmentsMutable() { return Fragments; }
	UPlayground_InventoryItem* Manifest(UObject* NewOuter);
	EPlayground_ItemCategory GetItemCategory() const { return ItemCategory; }
	FGameplayTag GetItemType() const { return ItemType; }
	void AssimilateInventoryFragments(UPlayground_CompositeBase* Composite) const;

	template<typename T> requires std::derived_from<T, FPlayground_ItemFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const;

	template<typename T> requires std::derived_from<T, FPlayground_ItemFragment>
	const T* GetFragmentOfType() const;

	template<typename T> requires std::derived_from<T, FPlayground_ItemFragment>
	T* GetFragmentOfTypeMutable();

	template<typename T> requires std::derived_from<T, FPlayground_ItemFragment>
	TArray<const T*> GetAllFragmentsOfType() const;

	void PG_SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation);

	UPROPERTY(EditDefaultsOnly)
	FName ItemID;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FPlayground_ItemFragment>> Fragments;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	EPlayground_ItemCategory ItemCategory{ EPlayground_ItemCategory::None };

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "GameItems"))
	FGameplayTag ItemType;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<AActor> PickupActorClass;
	
	void PG_ClearFragments();
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

template<typename T> requires std::derived_from<T, FPlayground_ItemFragment>
const T* FPlayground_ItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FPlayground_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}

	return nullptr;
}

template<typename T> requires std::derived_from<T, FPlayground_ItemFragment>
T* FPlayground_ItemManifest::GetFragmentOfTypeMutable()
{
	for (TInstancedStruct<FPlayground_ItemFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr;
		}
	}

	return nullptr;
}

template<typename T> requires std::derived_from<T, FPlayground_ItemFragment>
TArray<const T*> FPlayground_ItemManifest::GetAllFragmentsOfType() const
{
	TArray<const T*> Result;
	for (const TInstancedStruct<FPlayground_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			Result.Add(FragmentPtr);
		}
	}

	return Result;
}
