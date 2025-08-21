// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "PlaygroundBaseCharacter.generated.h"


class UPlaygroundAbilitySystemComponent;
class UPlaygroundAttributeSet;
class UDataAsset_StartUpDataBase;

UCLASS()
class PROJECT_V_API APlaygroundBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlaygroundBaseCharacter();

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	//~ End IAbilitySystemInterface Interface

protected:

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem");
	UPlaygroundAbilitySystemComponent* PlaygroundAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem");
	UPlaygroundAttributeSet* PlaygroundAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData");
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

public: 
	FORCEINLINE UPlaygroundAbilitySystemComponent* GetPlaygroundAbilitySystemComponent() const { return PlaygroundAbilitySystemComponent; }
	
	FORCEINLINE UPlaygroundAttributeSet* GetPlaygroundAttributeSet() const { return PlaygroundAttributeSet; }
	
};
