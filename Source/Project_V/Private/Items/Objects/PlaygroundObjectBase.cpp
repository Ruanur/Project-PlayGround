// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Objects/PlaygroundObjectBase.h"
#include "Characters/PlaygroundPlayerCharacter.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "PlaygroundGameplayTags.h"


void APlaygroundObjectBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlaygroundPlayerCharacter* OverrlappedPlayerCharacter = Cast<APlaygroundPlayerCharacter>(OtherActor))
	{
		OverrlappedPlayerCharacter->GetPlaygroundAbilitySystemComponent()->TryActivateAbilityByTag(PlaygroundGameplayTags::Player_Ability_Interact_Object);
		// Blueprint 이벤트 호출
	}
}
