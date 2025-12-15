

#include "Items/Drops/Playground_ItemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values

UPlayground_ItemComponent::UPlayground_ItemComponent()
{
}

void UPlayground_ItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}

void UPlayground_ItemComponent::PickedUp()
{
	OnPickedUp();
	GetOwner()->Destroy();
}