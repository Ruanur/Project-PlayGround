// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/PickUps/PlaygroundPickUpBase.h"
#include "Components/SphereComponent.h"

APlaygroundPickUpBase::APlaygroundPickUpBase()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Pick Up Interaction"));
	SetRootComponent(InteractionSphere);
	InteractionSphere->InitSphereRadius(50.f);
	InteractionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnPickUpCollisionSphereBeginOverlap);
}

void APlaygroundPickUpBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}


