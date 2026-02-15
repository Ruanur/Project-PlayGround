// Copyright 2025. Jinsol Co. All rights reserved


#include "Transfer/Playground_LevelTransfer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlaygroundPlayerCharacter.h"

#include "PlaygroundDebugHelper.h"

// Sets default values
APlayground_LevelTransfer::APlayground_LevelTransfer()
{
	PrimaryActorTick.bCanEverTick = false;

	TransferVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransferVolume"));
	RootComponent = TransferVolume;

	TransferVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TransferVolume->SetCollisionObjectType(ECC_WorldDynamic);
	TransferVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	TransferVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void APlayground_LevelTransfer::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayground_LevelTransfer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	APlaygroundPlayerCharacter* Character = Cast<APlaygroundPlayerCharacter>(OtherActor);
	if (bIsTransferring) return;
	if (!Character) return;
	if (LevelToLoad.IsNull())
	{
		Debug::Print(TEXT("[LevelTransfer] LevelToLoad is Not Assigned!"));
		return;
	}

	bIsTransferring = true;

	Character->CharacterRequestSaveInventory();

	FSoftObjectPath LevelPath = LevelToLoad.ToSoftObjectPath();
	FString PackageName = LevelPath.GetLongPackageName();

	if (PackageName.IsEmpty())
	{
		Debug::Print(TEXT("Invalid Level Path from ") + GetName());
		return;
	}

	FName LevelFName(*PackageName);

	UGameplayStatics::OpenLevel(GetWorld(), LevelFName);

	//if (Character)
	//{
	//	UGameplayStatics::OpenLevel(GetWorld(), FName(TransferLevelName));
	//}
}

