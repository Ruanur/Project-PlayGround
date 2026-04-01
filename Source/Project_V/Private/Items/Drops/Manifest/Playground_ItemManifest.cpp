// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Drops/Manifest/Playground_ItemManifest.h"

#include "Items/Drops/Playground_InventoryItem.h"
#include "Items/Drops/Playground_ItemComponent.h"
#include "Items/Fragment/Playground_ItemFragment.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "Widgets/Composite/Playground_CompositeBase.h"

UPlayground_InventoryItem* FPlayground_ItemManifest::Manifest(UObject* NewOuter)
{
	return Manifest(NewOuter, GetConfiguredRarity());
}

UPlayground_InventoryItem* FPlayground_ItemManifest::Manifest(UObject* NewOuter, EPlaygroundRarity InRarity)
{
	Debug::Print(
		FString::Printf(TEXT("Manifest InRarity = %d"), static_cast<int32>(InRarity)),
		FColor::Yellow
	);

	UPlayground_InventoryItem* Item = NewObject<UPlayground_InventoryItem>(NewOuter, UPlayground_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);

	FPlayground_ItemManifest& ItemManifest = Item->GetItemManifestMutable();

	//float RarityMultiplier = 1.f;

	//InRarity = GetConfiguredRarity();

	if (FPlayground_ItemRarity* RarityFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_ItemRarity>())
	{
		RarityFragment->SetRarity(InRarity);
		//RarityMultiplier = RarityFragment->GetStatMultiplier();

		Debug::Print(FString::Printf(TEXT("Set Rarity = %d / Multiplier = %.2f"),
			static_cast<int32>(RarityFragment->GetRarity()),
			RarityFragment->GetStatMultiplier()), FColor::Green);
	}
	else
	{
		Debug::Print(TEXT("Manifest: RarityFragment not found"), FColor::Red);
	}

	// 각 Fragment 자기 값 초기화
	for (auto& Fragment : Item->GetItemManifestMutable().GetFragmentsMutable())
	{
		Fragment.GetMutable().Manifest();
	}

	// 현재 아이템 상태 기준 표시값 재계산
	ItemManifest.RefreshDisplayValueFromCurrentState();

	//FPlayground_LabeledNumberFragment* DisplayFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_LabeledNumberFragment>();

	//FPlayground_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_EquipmentFragment>();

	//if (DisplayFragment && EquipmentFragment)
	//{
	//	if (FPlayground_StrengthModifier* StrengthModifier = EquipmentFragment->GetStrengthModifierMutable())
	//	{
	//		const float FinalDisplayValue = StrengthModifier->GetValue() * RarityMultiplier;
	//		DisplayFragment->SetValue(FinalDisplayValue);
	//	}
	//	else if (FPlayground_BaseDamageModifier* BaseDamageModifier = EquipmentFragment->GetBaseDamageModifierMutable())
	//	{
	//		const float FinalDisplayValue = BaseDamageModifier->GetValue() * RarityMultiplier;
	//		DisplayFragment->SetValue(FinalDisplayValue);
	//	}
	//}

	PG_ClearFragments();

	return Item;
}

EPlaygroundRarity FPlayground_ItemManifest::GetConfiguredRarity() const
{
	if (const FPlayground_ItemRarity* RarityFragment = GetFragmentOfType<FPlayground_ItemRarity>())
	{
		return RarityFragment->GetRarity();
	}
	return EPlaygroundRarity::Common;
}

void FPlayground_ItemManifest::AssimilateInventoryFragments(UPlayground_CompositeBase* Composite) const
{
	const auto& InventoryItemFragments = GetAllFragmentsOfType<FPlayground_InventoryItemFragment>();
	for (const auto* Fragment : InventoryItemFragments)
	{
		Composite->ApplyFunction([Fragment](UPlayground_CompositeBase* Widget)
			{
				Fragment->Assimilate(Widget);
			});
	}
}

void FPlayground_ItemManifest::PG_SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (!IsValid(PickupActorClass) || !IsValid(WorldContextObject)) return;

	AActor* SpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickupActorClass, SpawnLocation, SpawnRotation);
	if (!IsValid(SpawnedActor)) return;

	// Set the item manifest, item category, item type, etc
	UPlayground_ItemComponent* ItemComp = SpawnedActor->FindComponentByClass<UPlayground_ItemComponent>();
	check(ItemComp);

	ItemComp->InitItemManifest(*this);
}

void FPlayground_ItemManifest::ApplySavedInstanceData(UPlayground_InventoryItem* Item, EPlaygroundRarity SavedRarity, bool bHasSavedBaseDamage, float SavedBaseDamageValue, bool bHasSavedStrenth, float SavedStrengthValue) const
{
	if (!IsValid(Item)) return;

	FPlayground_ItemManifest& ItemManifest = Item->GetItemManifestMutable();

	// 1. Rarity 복원
	//float RarityMultiplier = 1.f;

	if (FPlayground_ItemRarity* RarityFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_ItemRarity>())
	{
		RarityFragment->SetRarity(SavedRarity);
		//RarityMultiplier = RarityFragment->GetStatMultiplier();
	}

	// 2. 랜덤 Base Value 최종값 복원 (Ex. BaseDamage Fragment)
	//FPlayground_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_EquipmentFragment>();
	//
	//if (EquipmentFragment)
	//{
	//	if (bHasSavedBaseDamage)
	//	{
	//		if (FPlayground_BaseDamageModifier* BaseDamageModifier = EquipmentFragment->GetBaseDamageModifierMutable())
	//		{
	//			BaseDamageModifier->SetValue(SavedBaseDamageValue);
	//		}
	//	}
	//	if (bHasSavedStrenth)
	//	{
	//		if (FPlayground_StrengthModifier* StrengthModifier = EquipmentFragment->GetStrengthModifierMutable())
	//		{
	//			StrengthModifier->SetValue(SavedStrengthValue);
	//		}
	//	}
	//}

	if (bHasSavedBaseDamage)
	{
		if (FPlayground_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_EquipmentFragment>())
		{
			if (FPlayground_BaseDamageModifier* BaseDamageModifier = EquipmentFragment->GetBaseDamageModifierMutable())
			{
				BaseDamageModifier->SetValue(SavedBaseDamageValue);
			}
		}
	}

	if (bHasSavedStrenth)
	{
		if (FPlayground_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_EquipmentFragment>())
		{
			if (FPlayground_StrengthModifier* StrengthModifier = EquipmentFragment->GetStrengthModifierMutable())
			{
				StrengthModifier->SetValue(SavedStrengthValue);
			}
		}
	}


	// 3. 표시값 재배치
	ItemManifest.RefreshDisplayValueFromCurrentState();
	
	//FPlayground_LabeledNumberFragment* DisplayFragment = ItemManifest.GetFragmentOfTypeMutable<FPlayground_LabeledNumberFragment>();

	//if (DisplayFragment && EquipmentFragment)
	//{
	//	if (FPlayground_StrengthModifier* StrengthModifier = EquipmentFragment->GetStrengthModifierMutable())
	//	{
	//		const float FinalDisplayValue = StrengthModifier->GetValue() * RarityMultiplier;
	//		DisplayFragment->SetValue(FinalDisplayValue);
	//	}
	//	else if (FPlayground_BaseDamageModifier* BaseDamageModifier = EquipmentFragment->GetBaseDamageModifierMutable())
	//	{
	//		const float FinalDisplayValue = BaseDamageModifier->GetValue() * RarityMultiplier;
	//		DisplayFragment->SetValue(FinalDisplayValue);
	//	}
	//}
}

void FPlayground_ItemManifest::RefreshDisplayValueFromCurrentState()
{
	float RarityMultiplier = 1.f;

	if (FPlayground_ItemRarity* RarityFragment = GetFragmentOfTypeMutable<FPlayground_ItemRarity>())
	{
		RarityMultiplier = RarityFragment->GetStatMultiplier();
	}

	FPlayground_LabeledNumberFragment* DisplayFragment = GetFragmentOfTypeMutable<FPlayground_LabeledNumberFragment>();

	FPlayground_EquipmentFragment* EquipmentFragment = GetFragmentOfTypeMutable<FPlayground_EquipmentFragment>();

	if (!DisplayFragment || !EquipmentFragment) return;

	if (FPlayground_StrengthModifier* StrengthModifier = EquipmentFragment->GetStrengthModifierMutable())
	{
		const float FinalDisplayValue = StrengthModifier->GetValue() * RarityMultiplier;
		DisplayFragment->SetValue(FinalDisplayValue);
	}
	else if (FPlayground_BaseDamageModifier* BaseDamageModifier = EquipmentFragment->GetBaseDamageModifierMutable())
	{
		const float FinalDisplayValue = BaseDamageModifier->GetValue() * RarityMultiplier;
		DisplayFragment->SetValue(FinalDisplayValue);
	}
}

void FPlayground_ItemManifest::PG_ClearFragments()
{
	for (auto& Fragment : Fragments)
	{
		Fragment.Reset();
	}
	Fragments.Empty();
}
