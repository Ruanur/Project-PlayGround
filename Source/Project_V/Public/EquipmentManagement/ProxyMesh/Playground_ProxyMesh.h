// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Playground_ProxyMesh.generated.h"

class UPlayground_EquipmentComponent;
class USkeletalMeshComponent;

UCLASS()
class PROJECT_V_API APlayground_ProxyMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayground_ProxyMesh();

protected:
	virtual void BeginPlay() override;

private:
	
	// This is the mesh on the player-controlled Character
	TWeakObjectPtr<USkeletalMeshComponent> SourceMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPlayground_EquipmentComponent> EquipmentComponent;

	// This is the proxy mesh we will see in the Inventory Menu.
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;
};
