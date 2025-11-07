// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/PlaygroundPickUpBase.h"
#include "PlaygroundStoneBase.generated.h"


class UPlaygroundAbilitySystemComponent;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class PROJECT_V_API APlaygroundStoneBase : public APlaygroundPickUpBase
{
	GENERATED_BODY()
	
public:
	void Consume(UPlaygroundAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);

protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	//BlueprintImplementableEvent : 이벤트 훅, 파티클, 사운드, UI 갱신 등 블루프린트에서 처리
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
	void BP_OnStoneConsumed();

	//이 오브젝트가 적용할 GameplayEffect 클래스를 에디터에서 지정할 수 있도록 하는 변수
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;
};
