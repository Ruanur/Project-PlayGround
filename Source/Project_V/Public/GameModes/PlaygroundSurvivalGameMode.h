// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameModes/PlaygroundGameModeBase.h"
#include "PlaygroundSurvivalGameMode.generated.h"

class APlaygroundEnemyCharacter;

UENUM(BlueprintType)
enum class EPlaygroundSurvivalGameModeState : uint8
{
	WaitSpawnNewWave,
	SpawningNewWave,
	InProgress,
	WaveCompleted,
	AllWavesDone,
	PlayerDied,
	TimeOut
};

USTRUCT(BlueprintType)
struct FPlaygroundEnemyWaveSpawnerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<APlaygroundEnemyCharacter> SoftEnemyClassToSpawn;

	UPROPERTY(EditAnywhere)
	int32 MinPerSpawnCount = 1;

	UPROPERTY(EditAnywhere)
	int32 MaxPerSpawnCount = 3;
};

USTRUCT(BlueprintType)
struct FPlaygroundEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FPlaygroundEnemyWaveSpawnerInfo> EnemyWaveSpawnerDefinitions;

	UPROPERTY(EditAnywhere)
	int32 TotalEnemyToSpawnThisWave = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvivalGameModeStateChangedDelegate, EPlaygroundSurvivalGameModeState, CurrentState);
/**
 * 
 */
UCLASS()
class PROJECT_V_API APlaygroundSurvivalGameMode : public APlaygroundGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void SetCurrentSurvivalGameModeState(EPlaygroundSurvivalGameModeState InState);
	bool HasFinishedAllWaves() const;
	void PreLoadNextWaveEnemies();
	FPlaygroundEnemyWaveSpawnerTableRow* GetCurrentWaveSpawnerTableRow() const;
	int32 TrySpawnWaveEnemies();
	bool ShouldKeepSpawnEnemies() const;


	//카운트다운 함수 (제한 시간)
	FTimerHandle WaveCountDownTimerHandle;

	void StartWaveCountDown(float Duration);
	void HandleWaveCountDownTick();
	void FinishWaveCountDown();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Survival|Timer", meta = (AllowPrivateAccess = "true"))
	float TotalTime = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Survival|Timer", meta = (AllowPrivateAccess = "true"))
	float RemainingTime = 300.f;


	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);

	UPROPERTY()
	EPlaygroundSurvivalGameModeState CurrentSurvivalGameModeState;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSurvivalGameModeStateChangedDelegate OnSurvivalGameModeStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyWaveSpawnerDataTable;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 TotalWavesToSpawn;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveCount = 1;

	UPROPERTY()
	int32 CurrentSpawnedEnemiesCounter = 0;

	UPROPERTY()
	int32 TotalSpawnedEnemiesThisWaveCounter = 0;

	UPROPERTY()
	TArray<AActor*> TargetPointsArray;

	UPROPERTY()
	float TimePassedSinceStart = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnNewWaveWaitTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnEnemiesDelayTime = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float WaveCompletedWaitTime = 5.f;

	UPROPERTY()
	TMap<TSoftClassPtr<APlaygroundEnemyCharacter>, UClass*> PreLoadedEnemyClassMap;

public:
	UFUNCTION(BlueprintCallable)
	void RegisterSpawnedEnemies(const TArray<APlaygroundEnemyCharacter*>& InEnemiesToRegister);

	UFUNCTION(BlueprintPure)
	float GetRemainingTime() const { return RemainingTime; }

	UFUNCTION(BlueprintPure)
	float GetTotalTime() const { return TotalTime; }
}; 
