// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlaygroundFunctionLibrary.generated.h"

class UPlaygroundAbilitySystemComponent;

UENUM()
enum class EPlaygroundConfrimType : uint8
{
	Yes,
	No
};
/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlaygroundFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UPlaygroundAbilitySystemComponent* NativeGetPlaygroundASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Playground|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Playground|FunctionLibrary")
	static void RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Playground|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfrimType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EPlaygroundConfrimType& OutConfrimType);
};
