// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayergroundTypes/PlaygroundEnumTypes.h"
#include "Inventory/Save/Playground_FInventorySlotInfo.h"
#include "PlaygroundFunctionLibrary.generated.h"

class UPlaygroundAbilitySystemComponent;
class UPawnCombatComponent;
class UPlayground_InventoryGrid;
struct FScalableFloat;
class UPlaygroundGameInstance;
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
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Playground|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfrimType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EPlaygroundConfrimType& OutConfrimType);

	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Playground|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EPlaygroundValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "Playground|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "Playground|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel = 1.f);

	UFUNCTION(BlueprintPure, Category = "Playground|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference);

	UFUNCTION(BlueprintPure, Category = "Playground|FunctionLibrary")
	static bool IsValidBlock(AActor* InAttacker, AActor* InDefender);

	UFUNCTION(BlueprintCallable, Category = "Playground|FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Playground|FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput", TotalTime = "1.0", UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, 
		float& OutRemainingTime, EPlaygroundCountDownActionInput CountDownInput, 
		UPARAM(DisplayName = "Output") EPlaygroundCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintPure, Category = "Playground|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static UPlaygroundGameInstance* GetPlaygroundGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Playground|FunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(EPlaygroundInputMode InInputMode, const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Playground|FunctionLibrary")
	static void SaveCurrentGameDifficulty(EPlaygroundGameDifficulty InDifficultyToSave);

	UFUNCTION(BlueprintCallable, Category = "Playground|FunctionLibrary")
	static bool TryLoadSavedGameDifficulty(EPlaygroundGameDifficulty& OutSavedDifficutly);

	UFUNCTION(BlueprintCallable, Category = "Playground|FunctionLibrary")
	static void SaveCurrentLookSensitivity(float InYaw, float InPitch);

	UFUNCTION(BlueprintCallable, Category = "Playground|FunctionLibrary")
	static bool TryLoadSavedLookSensitivity(float& OutYaw, float& OutPitch);

	UFUNCTION(BlueprintCallable, Category = "Save|Inventory")
	static void SaveInventory(const TArray<FInventorySlotInfo>& Slots);

	UFUNCTION(BlueprintCallable, Category = "Save|Inventory")
	static bool TryLoadInventory(TArray<FInventorySlotInfo>& OutSlots);
};
