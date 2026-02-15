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
