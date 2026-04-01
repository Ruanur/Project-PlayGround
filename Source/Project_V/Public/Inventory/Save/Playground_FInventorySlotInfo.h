// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Drops/Playground_InventoryItem.h"
#include "Playground_FInventorySlotInfo.generated.h"

/**
 * 
 */

// SaveGame 직렬화 구조체 (Inventory) - 인벤토리
USTRUCT(BlueprintType)
struct FInventorySlotInfo
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    FName ItemID;
    //TObjectPtr<UPlayground_InventoryItem> Item;
    
    UPROPERTY(SaveGame)
    FGuid InstanceID;

    UPROPERTY(SaveGame)
    int32 Index = INDEX_NONE;

    UPROPERTY(SaveGame)
    int32 UpperLeftIndex = INDEX_NONE;

    UPROPERTY(SaveGame)
    bool bIsStackable = false;

    UPROPERTY(SaveGame)
    int32 StackAmount = 0;

    // =================
    // 인스턴스 상태 저장
    // =================
    UPROPERTY(SaveGame)
    EPlaygroundRarity SavedRarity = EPlaygroundRarity::Common;

    UPROPERTY(SaveGame)
    bool bHasSavedBaseDamage = false;

    UPROPERTY(SaveGame)
    bool bHasSavedStrengthDamage = false;

    UPROPERTY(SaveGame)
    float SavedBaseDamageValue = 0.f;

    UPROPERTY(SaveGame)
    float SavedStrengthValue = 0.f;

    // Default constructor
    FInventorySlotInfo() = default;

    // Convenience Constructor - matches what I use in Emplace 
    explicit FInventorySlotInfo(
        FName InItemID,
        FGuid InInstanceID,
        int32 InIndex,
        int32 InUpperLeftIndex,
        bool bInIsStackable,
        int32 InStack = 0,
        EPlaygroundRarity InSavedRarity = EPlaygroundRarity::Common,
        bool bInHasSavedBaseDamage = false,
        float InSavedBaseDamageValue = 0.f,
        bool bInHasSavedStrengthDamage = false,
        float InSavedStrengthValue = 0.f)
        : ItemID(InItemID)
        , InstanceID(InInstanceID)
        , Index(InIndex)
        , UpperLeftIndex(InUpperLeftIndex)
        , bIsStackable(bInIsStackable)
        , StackAmount(InStack)
        , SavedRarity(InSavedRarity)
        , bHasSavedBaseDamage(bInHasSavedBaseDamage)
        , SavedBaseDamageValue(InSavedBaseDamageValue)
        , bHasSavedStrengthDamage(bInHasSavedStrengthDamage)
        , SavedStrengthValue(InSavedStrengthValue)
    {
    }

};

// 장착 슬롯
USTRUCT(BlueprintType)
struct FEquippedSlotInfo
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    FGameplayTag EquipmentTypeTag;

    UPROPERTY(SaveGame)
    FName ItemID = NAME_None;

    UPROPERTY(SaveGame)
    FGuid InstanceID;

    UPROPERTY(SaveGame) 
    bool bIsStackable = false;

    UPROPERTY(SaveGame)
    int32 StackAmount = 1;

    // =================
    // 인스턴스 상태 저장
    // =================
    UPROPERTY(SaveGame)
    EPlaygroundRarity SavedRarity = EPlaygroundRarity::Common;

    UPROPERTY(SaveGame)
    bool bHasSavedBaseDamage = false;

    UPROPERTY(SaveGame)
    bool bHasSavedStrengthDamage = false;

    UPROPERTY(SaveGame)
    float SavedBaseDamageValue = 0.f;

    UPROPERTY(SaveGame)
    float SavedStrengthValue = 0.f;

    FEquippedSlotInfo() = default;

    FEquippedSlotInfo(
        const FGameplayTag& InEquipmentTypeTag,
        FName InItemID,
        const FGuid& InInstanceID,
        bool bInIsStackable,
        int32 InStackAmount,
        EPlaygroundRarity InSavedRarity = EPlaygroundRarity::Common,
        bool bInHasSavedBaseDamage = false,
        float InSavedBaseDamageValue = 0.f,
        bool bHasSavedStrengthDamage = false,
        float SavedStrengthValue = 0.f)
        : EquipmentTypeTag(InEquipmentTypeTag)
        , ItemID(InItemID)
        , InstanceID(InInstanceID)
        , bIsStackable(bInIsStackable)
        , StackAmount(InStackAmount)
        , SavedRarity(InSavedRarity)
        , bHasSavedBaseDamage(bInHasSavedBaseDamage)
        , SavedBaseDamageValue(InSavedBaseDamageValue)
        , bHasSavedStrengthDamage(bHasSavedStrengthDamage)
        , SavedStrengthValue(SavedStrengthValue)
    {
    }
};
