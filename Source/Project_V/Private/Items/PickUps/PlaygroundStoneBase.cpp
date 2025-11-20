// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/PickUps/PlaygroundStoneBase.h"
#include "Characters/PlaygroundPlayerCharacter.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "PlaygroundGameplayTags.h"


void APlaygroundStoneBase::Consume(UPlaygroundAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	//지정된 효과 클래스가 없으면 크래시 발생, 방어 장치
	check(StoneGameplayEffectClass);

	//이 코드를 통해 해당 오브젝트의 GE 블루프린트 가져옴
	UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	//획득자에게 지정된 GameplayEffect 적용
	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		EffectCDO,
		ApplyLevel,
		AbilitySystemComponent->MakeEffectContext()
	);

	BP_OnStoneConsumed();
}

void APlaygroundStoneBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlaygroundPlayerCharacter* OverrlappedPlayerCharacter = Cast<APlaygroundPlayerCharacter>(OtherActor))
	{
		//플레이어 태그 활성화 시도
		OverrlappedPlayerCharacter->GetPlaygroundAbilitySystemComponent()->TryActivateAbilityByTag(PlaygroundGameplayTags::Player_Ability_PickUp_Stones);
	}
}

