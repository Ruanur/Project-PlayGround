

#include "Items/Drops/Playground_ItemComponent.h"
#include "Net/UnrealNetwork.h"

#include "PlaygroundDebugHelper.h"

// Sets default values

UPlayground_ItemComponent::UPlayground_ItemComponent()
{
}

void UPlayground_ItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
	DOREPLIFETIME(ThisClass, ItemRarity);
}

void UPlayground_ItemComponent::InitItemManifest(const FPlayground_ItemManifest& CopyOfManifest)
{
	ItemManifest = CopyOfManifest;
	ItemRarity = ItemManifest.GetConfiguredRarity();
	Debug::Print(
		FString::Printf(TEXT("InitItemManifest: ConfiguredRarity=%d, ItemID=%s"),
			static_cast<int32>(CopyOfManifest.GetConfiguredRarity()),
			*CopyOfManifest.ItemID.ToString()),
		FColor::Yellow
	);
}

void UPlayground_ItemComponent::PickedUp()
{
	OnPickedUp();
	GetOwner()->Destroy();
}