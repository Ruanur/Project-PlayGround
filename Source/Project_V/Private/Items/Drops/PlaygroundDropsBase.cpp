// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Drops/PlaygroundDropsBase.h"
#include "Characters/PlaygroundPlayerCharacter.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "Inventory/Playground_InventoryComponent.h"
#include "Items/Drops/Playground_ItemComponent.h"
#include "PlaygroundGameplayTags.h"

#include "PlaygroundDebugHelper.h"
void APlaygroundDropsBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlaygroundPlayerCharacter* Player = Cast<APlaygroundPlayerCharacter>(OtherActor);
	if (!Player) return;

	UPlayground_InventoryComponent* Inv = Player->FindComponentByClass<UPlayground_InventoryComponent>();
	if (!Inv) return;

	UPlayground_ItemComponent* ItemComp = FindComponentByClass<UPlayground_ItemComponent>();
	if (!ItemComp) return;

	//Player Inventory 컴포넌트의 TryAddItem 호출, 정상 작동
	Inv->TryAddItem(ItemComp);

	Debug::Print(TEXT("Called This Func"));
}

void APlaygroundDropsBase::InitializeDropFromRarity(EPlaygroundRarity InRarity)
{
	DroppedRarity = InRarity;

	Debug::Print(
		FString::Printf(TEXT("InitializeDropFromRarity ActorClass = %s"), *GetClass()->GetName()),
		FColor::Yellow
	);

	UPlayground_ItemComponent* ItemComp = FindComponentByClass<UPlayground_ItemComponent>();
	if (!ItemComp)
	{
		Debug::Print(TEXT("InitFromRarity: Item Comp not Found"), FColor::Red);
		return;
	}

	Debug::Print(TEXT("InitFromRarity: Item Comp Found"), FColor::Green);

	// 기존 ItemComponent에 세팅된 원본 Manifest를 복사해서 사용
	FPlayground_ItemManifest ManifestCopy = ItemComp->GetItemManifest();

	Debug::Print(
		FString::Printf(
			TEXT("Source Manifest ItemID = %s, Source Rarity = %d"),
			*ManifestCopy.ItemID.ToString(),
			static_cast<int32>(ManifestCopy.GetConfiguredRarity())
		),
		FColor::Cyan
	);

	UPlayground_InventoryItem* CreatedItem = ManifestCopy.Manifest(this, InRarity);

	if (!CreatedItem)
	{
		Debug::Print(TEXT("InitializeDropFromRarity: Failed to create item"), FColor::Red);
		return;
	}


	// 최종적으로 희귀도 반영이 끝난 Manifest를 ItemComponent에 저장
	ItemComp->InitItemManifest(CreatedItem->GetItemManifest());

	Debug::Print(
		FString::Printf(
			TEXT("Drop initialized. ItemID=%s, Rarity=%d"),
			*CreatedItem->GetItemManifest().ItemID.ToString(),
			static_cast<int32>(InRarity)
		),
		FColor::Green
	);
}
