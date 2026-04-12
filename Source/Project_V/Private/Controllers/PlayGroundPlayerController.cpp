// Copyright 2025. Jinsol Co. All rights reserved


#include "Controllers/PlayGroundPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/DamageText/Playground_DamageTextComponent.h"

#include "PlaygroundDebugHelper.h"


APlayGroundPlayerController::APlayGroundPlayerController()
{
	PlayerTeamID = FGenericTeamId(0);


}


FGenericTeamId APlayGroundPlayerController::GetGenericTeamId() const
{
	return PlayerTeamID;
}

void APlayGroundPlayerController::ShowDamageNumber_Implementation(float DamageAmount, AActor* TargetActor)
{
	if (IsValid(TargetActor) && DamageTextComponentClass)
	{
		UPlayground_DamageTextComponent* DamageText = NewObject<UPlayground_DamageTextComponent>(TargetActor, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

