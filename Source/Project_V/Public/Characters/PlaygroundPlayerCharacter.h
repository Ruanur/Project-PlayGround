// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlaygroundBaseCharacter.h"
#include "PlaygroundPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class PROJECT_V_API APlaygroundPlayerCharacter : public APlaygroundBaseCharacter
{
	GENERATED_BODY()

public:
	APlaygroundPlayerCharacter();
	
protected:
	virtual void BeginPlay() override;
	
private:
#pragma region Coponents
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"));
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"));
	UCameraComponent* FollowCamera;

#pragma endregion
};
