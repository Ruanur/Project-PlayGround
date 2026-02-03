// Copyright 2025. Jinsol Co. All rights reserved


#include "EquipmentManagement/ProxyMesh/Playground_ProxyMesh.h"
#include "EquipmentManagement/Components/Playground_EquipmentComponent.h"

// Sets default values
APlayground_ProxyMesh::APlayground_ProxyMesh()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	EquipmentComponent = CreateDefaultSubobject<UPlayground_EquipmentComponent>("Equipment");
	EquipmentComponent->SetOwningSkeletalMesh(Mesh);
	EquipmentComponent->SetIsProxy(true);
}
 

void APlayground_ProxyMesh::BeginPlay()
{
	Super::BeginPlay();

	
}


