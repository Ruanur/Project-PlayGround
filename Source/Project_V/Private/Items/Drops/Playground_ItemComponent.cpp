// Copyright 2025. Jinsol Co. All rights reserved


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
