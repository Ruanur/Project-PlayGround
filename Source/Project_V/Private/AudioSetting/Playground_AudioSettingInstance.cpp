//// Copyright 2025. Jinsol Co. All rights reserved
//
//
//#include "AudioSetting/Playground_AudioSettingInstance.h"
//
//#include "SaveGame/PlaygroundSaveGame.h"
//#include "Kismet/GameplayStatics.h"
//#include "Math/UnrealMathUtility.h"
//#include "Sound/SoundClass.h"
//#include "Sound/SoundMix.h"
//
//void UPlayground_AudioSettingInstance::Init()
//{
//	Super::Init();
//
//	// 게임 시작 시 저장된 오디오 설정을 읽고
//	// 실제 사운드 시스템에 적용한다.
//	LoadAudioSettings();
//	ApplyAudioSettings();
//}
//
//void UPlayground_AudioSettingInstance::ApplyClassVolume(USoundClass* InSoundClass, float InVolume, bool bApplyToChildren)
//{
//	if (!OptionsSoundMix || !InSoundClass)
//	{
//		return;
//	}
//
//	// SoundMix 안의 Sound class 값을 런타임에 덮어씀
//	// 필요 시 해당 클래스 adjuster가 추가된다.
//	UGameplayStatics::SetSoundMixClassOverride(
//		this,
//		OptionsSoundMix,
//		InSoundClass,
//		ClampVolume(InVolume),
//		1.0f,
//		0.05f,
//		bApplyToChildren
//	);
//}
//
//float UPlayground_AudioSettingInstance::ClampVolume(float InVolume)
//{
//	// 슬라이더 값이나 외부 입력이 이상한 값으로 들어와도 0.0 ~ 1.0 범위 안으로 강제 보정
//	return FMath::Clamp(InVolume, 0.0f, 1.0f);
//}
//
//
//void UPlayground_AudioSettingInstance::ApplyAudioSettings()
//{
//	if (!OptionsSoundMix)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("UAudioInstance::ApplyAudioSettings - OptionsSoundMix is null."));
//		return;
//	}
//
//	// Sound Mix는 먼저 Push되어 있어야 override가 제대로 적용된다.
//	// 여러 번 Push 방지, 한 번만 호출되고자 함
//	if (!bSoundMixPushed)
//	{
//		UGameplayStatics::PushSoundMixModifier(this, OptionsSoundMix);
//		bSoundMixPushed = true;
//	}
//
//	// Master는 보통 하위 클래스 전체에 영향을 주도록 true
//	ApplyClassVolume(MasterSoundClass, MasterVolume, true);
//
//	// 카테고리별 세부 음량
//	ApplyClassVolume(BGMSoundClass, BGMVolume, false);
//	ApplyClassVolume(SFXSoundClass, SFXVolume, false);
//	ApplyClassVolume(UISoundClass, UIVolume, false);
//}
//
//void UPlayground_AudioSettingInstance::SaveAudioSettings()
//{
//	if (!CachedSaveGame)
//	{
//		CachedSaveGame = Cast<UPlaygroundSaveGame>(
//			UGameplayStatics::CreateSaveGameObject(UPlaygroundSaveGame::StaticClass())
//		);
//	}
//
//	if (!CachedSaveGame)
//	{
//		UE_LOG(LogTemp, Error, TEXT("UAudioInstance::SaveAudioSettings - Failed to create save object."));
//		return;
//	}
//
//	// 현재 런타임 값을 SaveGame 객체에 복사
//	CachedSaveGame->MasterVolume = MasterVolume;
//	CachedSaveGame->BGMVolume = BGMVolume;
//	CachedSaveGame->SFXVolume = SFXVolume;
//	CachedSaveGame->UIVolume = UIVolume;
//	CachedSaveGame->SlotName = SaveSlotName;
//	CachedSaveGame->UserIndex = SaveUserIndex;
//
//	// 슬롯 파일로 저장
//	const bool bSuccess = UGameplayStatics::SaveGameToSlot(
//		CachedSaveGame,
//		SaveSlotName,
//		SaveUserIndex
//	);
//
//	if (!bSuccess)
//	{
//		UE_LOG(LogTemp, Error, TEXT("UAudioInstance::SaveAudioSettings - SaveGameToSlot failed."));
//	}
//}
//
//void UPlayground_AudioSettingInstance::LoadAudioSettings()
//{
//	// 기존 저장 파일이 있으면 로드
//	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
//	{
//		CachedSaveGame = Cast<UPlaygroundSaveGame>(
//			UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex)
//		);
//	}
//
//	// 저장 파일이 없거나 로드 실패한 경우
//	if (!CachedSaveGame)
//	{
//		// 새 SaveGame 객체 생성
//		CachedSaveGame = Cast<UPlaygroundSaveGame>(
//			UGameplayStatics::CreateSaveGameObject(UPlaygroundSaveGame::StaticClass())
//		);
//
//		if (!CachedSaveGame)
//		{
//			UE_LOG(LogTemp, Error, TEXT("UAudioInstance::LoadAudioSettings - Failed to create save object."));
//			ResetAudioSettings(false);
//			return;
//		}
//
//		CachedSaveGame->SlotName = SaveSlotName;
//		CachedSaveGame->UserIndex = SaveUserIndex;
//
//		// 기본값 적용
//		MasterVolume = ClampVolume(DefaultMasterVolume);
//		BGMVolume = ClampVolume(DefaultBGMVolume);
//		SFXVolume = ClampVolume(DefaultSFXVolume);
//		UIVolume = ClampVolume(DefaultUIVolume);
//
//		// 첫 실행이라면 기본값으로 저장 파일도 만든다
//		SaveAudioSettings();
//		return;
//	}
//
//	// 저장된 값 로드
//	MasterVolume = ClampVolume(CachedSaveGame->MasterVolume);
//	BGMVolume = ClampVolume(CachedSaveGame->BGMVolume);
//	SFXVolume = ClampVolume(CachedSaveGame->SFXVolume);
//	UIVolume = ClampVolume(CachedSaveGame->UIVolume);
//}
//
//void UPlayground_AudioSettingInstance::ResetAudioSettings(bool bSaveNow)
//{
//	// 에디터에서 지정한 기본값으로 되돌림
//	MasterVolume = ClampVolume(DefaultMasterVolume);
//	BGMVolume = ClampVolume(DefaultBGMVolume);
//	SFXVolume = ClampVolume(DefaultSFXVolume);
//	UIVolume = ClampVolume(DefaultUIVolume);
//
//	// 실제 오디오 시스템에 즉시 반영
//	ApplyAudioSettings();
//
//	// 필요하면 저장까지 수행
//	if (bSaveNow)
//	{
//		SaveAudioSettings();
//	}
//}
//
//void UPlayground_AudioSettingInstance::SetMasterVolume(float NewVolume, bool bSaveNow)
//{
//	MasterVolume = ClampVolume(NewVolume);
//	ApplyAudioSettings();
//
//	if (bSaveNow)
//	{
//		SaveAudioSettings();
//	}
//}
//
//void UPlayground_AudioSettingInstance::SetBGMVolume(float NewVolume, bool bSaveNow)
//{
//	BGMVolume = ClampVolume(NewVolume);
//	ApplyAudioSettings();
//
//	if (bSaveNow)
//	{
//		SaveAudioSettings();
//	}
//}
//
//void UPlayground_AudioSettingInstance::SetSFXVolume(float NewVolume, bool bSaveNow)
//{
//	SFXVolume = ClampVolume(NewVolume);
//	ApplyAudioSettings();
//
//	if (bSaveNow)
//	{
//		SaveAudioSettings();
//	}
//}
//
//void UPlayground_AudioSettingInstance::SetUIVolume(float NewVolume, bool bSaveNow)
//{
//	UIVolume = ClampVolume(NewVolume);
//	ApplyAudioSettings();
//
//	if (bSaveNow)
//	{
//		SaveAudioSettings();
//	}
//}
