// Copyright 2025. Jinsol Co. All rights reserved


#include "PlaygroundGameInstance.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

#include "PlaygroundDebugHelper.h"


void UPlaygroundGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnDestinationWorldLoaded);

	Debug::Print(TEXT("Init Run"));
}

void UPlaygroundGameInstance::OnPreLoadMap(const FString& MapName)
{
	const auto LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass.LoadSynchronous(), TEXT("LoadingScreen"));
	TSharedRef<SWidget> LoadingSWidgetPtr = LoadingWidget->TakeWidget();

	FLoadingScreenAttributes LoadingScreenAttributes;

	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;

	//로딩 화면 최소 2초 유지
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;

	//LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	LoadingScreenAttributes.WidgetLoadingScreen = LoadingSWidgetPtr;

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);

	Debug::Print(TEXT("OnPreLoadMap Run"));
}

void UPlaygroundGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
	//로딩 종료
	GetMoviePlayer()->StopMovie();
	Debug::Print(TEXT("OnDestinationWorldLoaded Run"));
}

TSoftObjectPtr<UWorld> UPlaygroundGameInstance::GetLevel(FString LevelName)
{
	if (MapList.IsEmpty())
	{
		Debug::Print(TEXT("Map List Is Null Please Fill"));
		return nullptr;
	}

	return *MapList.Find(LevelName);
}


TSoftObjectPtr<UWorld> UPlaygroundGameInstance::GetGameLevelByTag(FGameplayTag InTag) const
{
	for (const FPlaygroundGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if (!GameLevelSet.IsValid()) continue;

		if (GameLevelSet.LevelTag == InTag)
		{
			return GameLevelSet.Level;
		}
	}

	return TSoftObjectPtr<UWorld>();
}
