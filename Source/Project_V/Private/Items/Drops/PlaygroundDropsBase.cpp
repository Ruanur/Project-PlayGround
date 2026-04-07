// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Drops/PlaygroundDropsBase.h"
#include "Characters/PlaygroundPlayerCharacter.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "Inventory/Playground_InventoryComponent.h"
#include "Items/Drops/Playground_ItemComponent.h"
#include "PlaygroundGameplayTags.h"
#include "Components/MeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Components/PrimitiveComponent.h"

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

	ApplyOutlineMaterialByRarity(InRarity);
	ApplySpawnImpulse();

	Debug::Print(
		FString::Printf(
			TEXT("Drop initialized. ItemID=%s, Rarity=%d"),
			*CreatedItem->GetItemManifest().ItemID.ToString(),
			static_cast<int32>(InRarity)
		),
		FColor::Green
	);
}

UMeshComponent* APlaygroundDropsBase::FindOutlineTargetMesh() const
{
	TArray<UMeshComponent*> MeshComponents;
	GetComponents<UMeshComponent>(MeshComponents);

	for (UMeshComponent* MeshComp : MeshComponents)
	{
		if (IsValid(MeshComp))
		{
			return MeshComp;
		}
	}
	return nullptr;
}

UMaterialInterface* APlaygroundDropsBase::GetOutlineMaterialByRarity(EPlaygroundRarity InRarity) const
{
	switch (InRarity)
	{
	case EPlaygroundRarity::Common:
		return OutlineMaterial_Common;
	case EPlaygroundRarity::Uncommon:
		return OutlineMaterial_Uncommon;
	case EPlaygroundRarity::Rare:
		return OutlineMaterial_Rare;
	case EPlaygroundRarity::Epic:
		return OutlineMaterial_Epic;
	case EPlaygroundRarity::Legendary:
		return OutlineMaterial_Legendary;
	default:
		return OutlineMaterial_Common;
	}
}

void APlaygroundDropsBase::ApplyOutlineMaterialByRarity(EPlaygroundRarity InRarity)
{
	UMeshComponent* MeshComp = FindOutlineTargetMesh();
	if (!MeshComp)
	{
		return;
	}

	UMaterialInterface* OutlineMat = GetOutlineMaterialByRarity(InRarity);
	if (!OutlineMat)
	{
		return;
	}

	MeshComp->SetOverlayMaterial(OutlineMat);
}

UPrimitiveComponent* APlaygroundDropsBase::FindDropPhysicsComponent() const
{
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents);

	for (UPrimitiveComponent* PrimComp : PrimitiveComponents)
	{
		if (!IsValid(PrimComp)) continue;

		// 보조 컴포넌트 제외 시
		// SimulatePhysics 가능한 컴포넌트만 찾음
		if (PrimComp->IsSimulatingPhysics())
		{
			return PrimComp;
		}
	}

	return nullptr;
}

void APlaygroundDropsBase::ApplySpawnImpulse()
{
	if (!bApplySpawnImpulse) return;

	UPrimitiveComponent* PrimComp = FindDropPhysicsComponent();
	if (!PrimComp)
	{
		Debug::Print(TEXT("ApplySpawnImpulse: No Physics component found"), FColor::Red);
		return;
	}
	
	if (!PrimComp->IsSimulatingPhysics())
	{
		Debug::Print(TEXT("ApplySpawnImpulse: Physics simulation is disables"), FColor::Red);
		return;
	}

	const float RandomX = FMath::FRandRange(-SpawnImpulseHorizontal, SpawnImpulseHorizontal);
	const float RandomY = FMath::FRandRange(-SpawnImpulseHorizontal, SpawnImpulseHorizontal);

	const FVector Impulse(RandomX, RandomY, SpawnImpulseUpward);

	PrimComp->AddImpulse(Impulse, NAME_None, true);
	PrimComp->WakeAllRigidBodies();

	Debug::Print(
		FString::Printf(TEXT("ApplySpawnImpulse: Impulse = X %.2f Y %.2f Z %.2f"), RandomX, RandomY, SpawnImpulseUpward),
		FColor::Green
	);
}
