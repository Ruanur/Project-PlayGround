// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "PlayGroundPlayerController.generated.h"

/**
 * 
 */

class UPlayground_InventoryGrid;
class UPlayground_DamageTextComponent;

UCLASS()
class PROJECT_V_API APlayGroundPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	APlayGroundPlayerController();

	//~ Begin IGenericTeamAgentInterface Interface.
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End IGenericTeamAgentInterface Interface

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, AActor* TargetCharacter);

private:
	FGenericTeamId PlayerTeamID;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayground_DamageTextComponent> DamageTextComponentClass;
};
