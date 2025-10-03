// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PlaygroundPlayerGameplayAbility.h"
#include "PlayerGameplayAbility_TargetLock.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlayerGameplayAbility_TargetLock : public UPlaygroundPlayerGameplayAbility
{
	GENERATED_BODY()

protected:
	// ~Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~End UGameplayAbility Interface

private:
	void TryLockOnTarget();
	void GetAvailableActorsToLock();
	
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);

	void CancelTargetLockAbility();
	void CleanUp();

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float BoxTraceDistance = 5000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannal;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	bool bShowPersistanceDebugShape = false;
	
	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;

	UPROPERTY()
	AActor* CurrentLockedActor;
};
