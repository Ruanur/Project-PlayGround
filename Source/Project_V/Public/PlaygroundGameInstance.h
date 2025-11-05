// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "PlaygroundGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FPlaygroundGameLevelSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "GameData.Level"))
	FGameplayTag LevelTag;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	bool IsValid() const
	{
		return LevelTag.IsValid() && !Level.IsNull();
	}

};

/**
 * 
 */
UCLASS()
class PROJECT_V_API UPlaygroundGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	TSoftObjectPtr<UWorld> GetLevel(FString LevelName);

protected:
	//게임 인스턴스가 처음 생성될 때 호출
	virtual void Init() override;

	//맵 로드가 시작되기 전에 로딩 화면 설정
	UFUNCTION()
	virtual void OnPreLoadMap(const FString& MapName);

	//맵이 로드된 후 로딩 화면을 종료
	UFUNCTION()
	virtual void OnDestinationWorldLoaded(UWorld* LoadedWorld);

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> LoadingWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPlaygroundGameLevelSet> GameLevelSets;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TMap<FString, TSoftObjectPtr<UWorld>> MapList;

public:
	UFUNCTION(BlueprintPure, meta = (GameplayTagFliter = "GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag InTag) const;
	
};
