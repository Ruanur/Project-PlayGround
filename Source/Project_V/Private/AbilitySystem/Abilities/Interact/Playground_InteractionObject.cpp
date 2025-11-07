// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/Abilities/Interact/Playground_InteractionObject.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/PlaygroundPlayerCharacter.h"
#include "Items/Objects/PlaygroundObjectBase.h"
#include "Components/UI/PlayerUIComponent.h"

void UPlayground_InteractionObject::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	GetPlayerUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(true);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPlayground_InteractionObject::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetPlayerUIComponentFromActorInfo()->OnStoneInteracted.Broadcast(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPlayground_InteractionObject::InteractObject()
{
	//APlaygroundObjectBase* ObjectsBase = nullptr;
	FHitResult HitResult;

	//위치 조정 필요
	bool bHit = UKismetSystemLibrary::BoxTraceSingleForObjects(
		GetPlayerCharacterFromActorInfo(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation() + GetPlayerCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		GetPlayerCharacterFromActorInfo()->GetActorRotation(),
		InteractableTraceChannel,
		false,
		TArray<AActor*>(),
		bDrawDebugShape ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		HitResult,
		true
	);

	//상호작용 시 BP_ObjectInteracted(블루프린트 함수) 전달
	if (bHit)
	{
		if (APlaygroundObjectBase* TargetObject = Cast<APlaygroundObjectBase>(HitResult.GetActor()))
		{
			TargetObject->BP_ObjectInteracted(GetPlayerCharacterFromActorInfo());
		}
	}
}


