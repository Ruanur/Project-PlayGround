//// Copyright 2025. Jinsol Co. All rights reserved
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Engine/GameInstance.h"
//#include "Playground_AudioSettingInstance.generated.h"
//
///**
// * 
// */
//
//class USoundMix;
//class USoundClass;
//class UPlaygroundSaveGame;
//
//UCLASS()
//class PROJECT_V_API UPlayground_AudioSettingInstance : public UGameInstance
//{
//	GENERATED_BODY()
//
//public:
//	virtual void Init() override;
//
//	// ===================
//	// 블루프린트 호출용 함수
//	// ===================
//
//	// 현재 저장된 음량 값을 Sound Mix에 반영
//	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
//	void ApplyAudioSettings();
//
//	// 현재 음량 값을 SaveGame 슬롯에 저장
//	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
//	void SaveAudioSettings();
//
//
//	// SaveGame 슬롯에서 음량 값을 읽어옴
//	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
//	void LoadAudioSettings();
//
//
//	// 기본값으로 되돌림
//	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
//	void ResetAudioSettings(bool bSaveNow = true);
//
//
//	// 마스터 음량 설정
//	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
//	void SetMasterVolume(float NewVolume, bool bSaveNow = true);
//
//
//	// 배경음 음량 설정
//	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
//	void SetBGMVolume(float NewVolume, bool bSaveNow = true);
//
//
//	// 효과음 음량 설정
//	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
//	void SetSFXVolume(float NewVolume, bool bSaveNow = true);
//
//	// UI 음량 설정
//	UFUNCTION(BlueprintCallable, Category = "Audio Settings")
//	void SetUIVolume(float NewVolume, bool bSaveNow = true);
//
//
//	// 현재 마스터 음량값 반환
//	UFUNCTION(BlueprintPure, Category = "Audio Settings")
//	float GetMasterVolume() const { return MasterVolume; }
//
//	// 현재 배경음 음량값 반환
//	UFUNCTION(BlueprintPure, Category = "Audio Settings")
//	float GetBGMVolume() const { return BGMVolume; }
//
//	// 현재 효과음량값 반환
//	UFUNCTION(BlueprintPure, Category = "Audio Settings")
//	float GetSFXVolume() const { return SFXVolume; }
//
//	// 현재 UI 음량값 반환
//	UFUNCTION(BlueprintPure, Category = "Audio Settings")
//	float GetUIVolume() const { return UIVolume; }
//
//protected:
//	// =========================
//	// 에디터에서 연결할 오디오 에셋
//	// =========================
//	
//	// 옵션용 SoundMix
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings|Assets")
//	TObjectPtr<USoundMix> OptionsSoundMix = nullptr;
//
//
//	// 전체 음량 Sound Class
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings|Assets")
//	TObjectPtr<USoundClass> MasterSoundClass = nullptr;
//
//	// 배경음 Sound Class
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings|Assets")
//	TObjectPtr<USoundClass> BGMSoundClass = nullptr;
//
//	// 효과음 Sound Class
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings|Assets")
//	TObjectPtr<USoundClass> SFXSoundClass = nullptr;
//
//	// UI Sound Class
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings|Assets")
//	TObjectPtr<USoundClass> UISoundClass = nullptr;
//
//
//	// =========================
//	// 저장 관련 설정
//	// =========================
//
//	// 세이브 슬롯 이름
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings|Save")
//	FString SaveSlotName = TEXT("AudioSettings");
//
//	// 유저 인덱스
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings|Save")
//	int32 SaveUserIndex = 0;
//
//
//	// =========================
//	// 기본값
//	// =========================
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings|Defaults", meta = (ClampMin = "0.0", ClampMax = "1.0"))
//	float DefaultMasterVolume = 1.0f;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings|Defaults", meta = (ClampMin = "0.0", ClampMax = "1.0"))
//	float DefaultBGMVolume = 1.0f;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings|Defaults", meta = (ClampMin = "0.0", ClampMax = "1.0"))
//	float DefaultSFXVolume = 1.0f;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings|Defaults", meta = (ClampMin = "0.0", ClampMax = "1.0"))
//	float DefaultUIVolume = 1.0f;
//
//private:
//	// 실제 저장 객체를 캐싱해두는 포인터
//	UPROPERTY(Transient)
//	TObjectPtr<UPlaygroundSaveGame> CachedSaveGame = nullptr;
//
//	// =========================
//	// 런타임 현재값
//	// =========================
//	
//	UPROPERTY(VisibleInstanceOnly, Category = "Audio Settings|Runtime")
//	float MasterVolume = 1.0f;
//
//	UPROPERTY(VisibleInstanceOnly, Category = "Audio Settings|Runtime")
//	float BGMVolume = 1.0f;
//
//	UPROPERTY(VisibleInstanceOnly, Category = "Audio Settings|Runtime")
//	float SFXVolume = 1.0f;
//
//	UPROPERTY(VisibleInstanceOnly, Category = "Audio Settings|Runtime")
//	float UIVolume = 1.0f;
//
//	// PushSoundMixModifier를 중복 호출하지 않기 위한 플래그
//	bool bSoundMixPushed = false;
//
//	void ApplyClassVolume(USoundClass* InSoundClass, float InVolume, bool bApplyToChildren);
//	static float ClampVolume(float InVolume);
//
//
//	
//};
