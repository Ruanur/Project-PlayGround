// Copyright 2025. Jinsol Co. All rights reserved


#include "PlaygroundGameInstance.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "SaveGame/PlaygroundSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

#include "PlaygroundDebugHelper.h"



void UPlaygroundGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnDestinationWorldLoaded);

	LoadAudioSettings();

	Debug::Print(TEXT("Init Run"));
}

void UPlaygroundGameInstance::OnPreLoadMap(const FString& MapName)
{
	bSoundMixPushed = false;

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
	
	ApplyAudioSettings();

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

void UPlaygroundGameInstance::ApplyAudioSettings()
{
	if (!OptionsSoundMix)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAudioInstance::ApplyAudioSettings - OptionsSoundMix is null."));
		return;
	}

//	// 테스트용:
//// Init 시점에 너무 일찍 Push되어 실제 오디오 시스템에 반영이 안 되는지 확인하려고
//// 매번 Push하도록 변경
//	UGameplayStatics::PushSoundMixModifier(this, OptionsSoundMix);
//
//	UE_LOG(LogTemp, Warning, TEXT("ApplyAudioSettings - PushSoundMixModifier Called"));
//
//	// Master는 자식 클래스까지 적용
//	if (MasterSoundClass)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("ApplyAudioSettings - Master: %s / %f"),
//			*MasterSoundClass->GetName(), MasterVolume);
//		ApplyClassVolume(MasterSoundClass, MasterVolume, true);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("ApplyAudioSettings - MasterSoundClass is null"));
//	}
//
//	// BGM
//	if (BGMSoundClass)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("ApplyAudioSettings - BGM: %s / %f"),
//			*BGMSoundClass->GetName(), BGMVolume);
//		ApplyClassVolume(BGMSoundClass, BGMVolume, false);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("ApplyAudioSettings - BGMSoundClass is null"));
//	}
//
//	// SFX
//	if (SFXSoundClass)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("ApplyAudioSettings - SFX: %s / %f"),
//			*SFXSoundClass->GetName(), SFXVolume);
//		ApplyClassVolume(SFXSoundClass, SFXVolume, false);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("ApplyAudioSettings - SFXSoundClass is null"));
//	}
//
//	// UI
//	if (UISoundClass)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("ApplyAudioSettings - UI: %s / %f"),
//			*UISoundClass->GetName(), UIVolume);
//		ApplyClassVolume(UISoundClass, UIVolume, false);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("ApplyAudioSettings - UISoundClass is null"));
//	}
//
//	Debug::Print(TEXT("Apply Audio Setting Called"));

	// Sound Mix는 먼저 Push되어 있어야 override가 제대로 적용된다.
	// 여러 번 Push 방지, 한 번만 호출되고자 함
	if (!bSoundMixPushed)
	{
		UGameplayStatics::PushSoundMixModifier(this, OptionsSoundMix);
		bSoundMixPushed = true;
	}

	// Master는 보통 하위 클래스 전체에 영향을 주도록 true
	ApplyClassVolume(MasterSoundClass, MasterVolume, true);

	// 카테고리별 세부 음량
	ApplyClassVolume(BGMSoundClass, BGMVolume, false);
	ApplyClassVolume(SFXSoundClass, SFXVolume, false);
	ApplyClassVolume(UISoundClass, UIVolume, false);

	Debug::Print(TEXT("Apply Audio Setting Called"));
}

void UPlaygroundGameInstance::SaveAudioSettings()
{
	if (!CachedSaveGame)
	{
		CachedSaveGame = Cast<UPlaygroundSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UPlaygroundSaveGame::StaticClass())
		);
	}

	if (!CachedSaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("UAudioInstance::SaveAudioSettings - Failed to create save object."));
		return;
	}

	// 현재 런타임 값을 SaveGame 객체에 복사
	CachedSaveGame->MasterVolume = MasterVolume;
	CachedSaveGame->BGMVolume = BGMVolume;
	CachedSaveGame->SFXVolume = SFXVolume;
	CachedSaveGame->UIVolume = UIVolume;
	CachedSaveGame->SlotName = SaveSlotName;
	CachedSaveGame->UserIndex = SaveUserIndex;

	// 슬롯 파일로 저장
	const bool bSuccess = UGameplayStatics::SaveGameToSlot(
		CachedSaveGame,
		SaveSlotName,
		SaveUserIndex
	);

	if (!bSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("UAudioInstance::SaveAudioSettings - SaveGameToSlot failed."));
	}
}

void UPlaygroundGameInstance::LoadAudioSettings()
{
	// 기존 저장 파일이 있으면 로드
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
	{
		CachedSaveGame = Cast<UPlaygroundSaveGame>(
			UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex)
		);
	}

	// 저장 파일이 없거나 로드 실패한 경우
	if (!CachedSaveGame)
	{
		// 새 SaveGame 객체 생성
		CachedSaveGame = Cast<UPlaygroundSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UPlaygroundSaveGame::StaticClass())
		);

		if (!CachedSaveGame)
		{
			UE_LOG(LogTemp, Error, TEXT("UAudioInstance::LoadAudioSettings - Failed to create save object."));
			ResetAudioSettings(false);
			return;
		}

		CachedSaveGame->SlotName = SaveSlotName;
		CachedSaveGame->UserIndex = SaveUserIndex;

		// 기본값 적용
		MasterVolume = ClampVolume(DefaultMasterVolume);
		BGMVolume = ClampVolume(DefaultBGMVolume);
		SFXVolume = ClampVolume(DefaultSFXVolume);
		UIVolume = ClampVolume(DefaultUIVolume);

		// 첫 실행이라면 기본값으로 저장 파일도 만든다
		SaveAudioSettings();
		return;
	}

	// 저장된 값 로드
	MasterVolume = ClampVolume(CachedSaveGame->MasterVolume);
	BGMVolume = ClampVolume(CachedSaveGame->BGMVolume);
	SFXVolume = ClampVolume(CachedSaveGame->SFXVolume);
	UIVolume = ClampVolume(CachedSaveGame->UIVolume);
}

void UPlaygroundGameInstance::ResetAudioSettings(bool bSaveNow)
{
	// 에디터에서 지정한 기본값으로 되돌림
	MasterVolume = ClampVolume(DefaultMasterVolume);
	BGMVolume = ClampVolume(DefaultBGMVolume);
	SFXVolume = ClampVolume(DefaultSFXVolume);
	UIVolume = ClampVolume(DefaultUIVolume);

	// 실제 오디오 시스템에 즉시 반영
	ApplyAudioSettings();

	// 필요하면 저장까지 수행
	if (bSaveNow)
	{
		SaveAudioSettings();
	}

	Debug::Print(TEXT("Reset Audio Settings"));
}

void UPlaygroundGameInstance::SetMasterVolume(float NewVolume, bool bSaveNow)
{
	MasterVolume = ClampVolume(NewVolume);
	ApplyAudioSettings();

	if (bSaveNow)
	{
		SaveAudioSettings();
	}
}

void UPlaygroundGameInstance::SetBGMVolume(float NewVolume, bool bSaveNow)
{
	BGMVolume = ClampVolume(NewVolume);
	ApplyAudioSettings();

	if (bSaveNow)
	{
		SaveAudioSettings();
	}

	Debug::Print(TEXT("Set BGM Volume"));
}

void UPlaygroundGameInstance::SetSFXVolume(float NewVolume, bool bSaveNow)
{
	SFXVolume = ClampVolume(NewVolume);
	ApplyAudioSettings();

	if (bSaveNow)
	{
		SaveAudioSettings();
	}
}

void UPlaygroundGameInstance::SetUIVolume(float NewVolume, bool bSaveNow)
{
	UIVolume = ClampVolume(NewVolume);
	ApplyAudioSettings();

	if (bSaveNow)
	{
		SaveAudioSettings();
	}
}


void UPlaygroundGameInstance::ApplyClassVolume(USoundClass* InSoundClass, float InVolume, bool bApplyToChildren)
{
	if (!OptionsSoundMix || !InSoundClass)
	{
		return;
	}

	// SoundMix 안의 Sound class 값을 런타임에 덮어씀
	// 필요 시 해당 클래스 adjuster가 추가된다.
	UGameplayStatics::SetSoundMixClassOverride(
		this,
		OptionsSoundMix,
		InSoundClass,
		ClampVolume(InVolume),
		1.0f,
		0.05f,
		bApplyToChildren
	);
}

float UPlaygroundGameInstance::ClampVolume(float InVolume)
{
	// 슬라이더 값이나 외부 입력이 이상한 값으로 들어와도 0.0 ~ 1.0 범위 안으로 강제 보정
	return FMath::Clamp(InVolume, 0.0f, 1.0f);
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
