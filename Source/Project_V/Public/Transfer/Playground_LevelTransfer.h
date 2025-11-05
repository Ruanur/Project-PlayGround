// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Playground_LevelTransfer.generated.h"

UCLASS()
class PROJECT_V_API APlayground_LevelTransfer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayground_LevelTransfer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(EditAnywhere, Category = "Level Transfer", meta = (AllowedClasses = "World", DisplayThumbnail = "true"))
	TSoftObjectPtr<UWorld> LevelToLoad;

	//UPROPERTY(EditAnywhere, meta = (AllowedClasses = "World", AllowPrivateAccess = "true"))
	//TSoftObjectPtr<UWorld> Level;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* TransferVolume;

	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	//FString TransferLevelName;

	// 로딩 상태 중복 방지
	bool bIsTransferring = false;

	// 내부 로드 완료 콜백
	void OnLevelLoaded();
};
