// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/PlaygroundPickUpBase.h"
#include "Characters/PlaygroundPlayerCharacter.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "PlaygroundDropsBase.generated.h"

/**
 * 
 */

class UPrimitiveComponent;

UCLASS()
class PROJECT_V_API APlaygroundDropsBase : public APlaygroundPickUpBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Object Interacted"))
	void BP_ObjectInteracted(APlaygroundPlayerCharacter* InteractingPlayer);

protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

#pragma region Loot_ItemManifest
public:
	UFUNCTION(BlueprintCallable, Category = "Drop")
	void InitializeDropFromRarity(EPlaygroundRarity InRarity);

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Drop")
	EPlaygroundRarity DroppedRarity = EPlaygroundRarity::Common;
#pragma endregion

#pragma region Outline
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Outline")
	TObjectPtr<UMaterialInterface> OutlineMaterial_Common = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Outline")
	TObjectPtr<UMaterialInterface> OutlineMaterial_Uncommon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Outline")
	TObjectPtr<UMaterialInterface> OutlineMaterial_Rare = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Outline")
	TObjectPtr<UMaterialInterface> OutlineMaterial_Epic = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Outline")
	TObjectPtr<UMaterialInterface> OutlineMaterial_Legendary = nullptr;

	UMeshComponent* FindOutlineTargetMesh() const;
	UMaterialInterface* GetOutlineMaterialByRarity(EPlaygroundRarity InRarity) const;
	void ApplyOutlineMaterialByRarity(EPlaygroundRarity InRarity);
#pragma endregion

#pragma region Drop_Animation
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drop|Impulse")
	bool bApplySpawnImpulse = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drop|Impulse", meta = (ClampMin = "0.0"))
	float SpawnImpulseUpward = 250.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drop|Impulse", meta = (ClampMin = "0.0"))
	float SpawnImpulseHorizontal = 80.f;

	UPrimitiveComponent* FindDropPhysicsComponent() const;
	void ApplySpawnImpulse();
#pragma endregion
};
