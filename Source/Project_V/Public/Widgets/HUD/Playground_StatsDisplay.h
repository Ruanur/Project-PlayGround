// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "GameplayEffectTypes.h"
#include "Playground_StatsDisplay.generated.h"

/**
 * 
 */

class UTextBlock;

UCLASS()
class PROJECT_V_API UPlayground_StatsDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EPlaygroundStatField StatField = EPlaygroundStatField::CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 FractionDigits = 0;
	

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Value;

	TWeakObjectPtr<UAbilitySystemComponent> CachedASC;
	FDelegateHandle PrimaryHandle;

	// Current/Max 같이 “둘 다 필요”한 표시를 원할 수도 있어서 확장용
	FDelegateHandle SecondaryHandle;

	UAbilitySystemComponent* ResolveASC() const;
	FGameplayAttribute GetAttributeForField(EPlaygroundStatField Field) const;
	FGameplayAttribute GetSecondaryAttributeIfNeeded(EPlaygroundStatField Field) const;

	void BindToASC(UAbilitySystemComponent* ASC);
	void UnbindFromASC();

	void RefreshText();
	void OnPrimaryChanged(const FOnAttributeChangeData& Data);
	void OnSecondaryChanged(const FOnAttributeChangeData& Data);

};
