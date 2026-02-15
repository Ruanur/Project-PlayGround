// Copyright 2025. Jinsol Co. All rights reserved


#include "PlaygroundFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/PawnCombatInterface.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlaygroundGameplayTags.h"
#include "PlayergroundTypes/PlaygroundCountDownAction.h"
#include "PlaygroundGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/PlaygroundSaveGame.h"
#include "Widgets/Spatial/Playground_InventoryGrid.h"

#include "PlaygroundDebugHelper.h"

UPlaygroundAbilitySystemComponent* UPlaygroundFunctionLibrary::NativeGetPlaygroundASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UPlaygroundAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UPlaygroundFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UPlaygroundAbilitySystemComponent* ASC = NativeGetPlaygroundASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UPlaygroundFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UPlaygroundAbilitySystemComponent* ASC = NativeGetPlaygroundASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}

}

bool UPlaygroundFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UPlaygroundAbilitySystemComponent* ASC = NativeGetPlaygroundASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UPlaygroundFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EPlaygroundConfrimType& OutConfrimType)
{
	OutConfrimType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EPlaygroundConfrimType::Yes : EPlaygroundConfrimType::No;
}

UPawnCombatComponent* UPlaygroundFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);
	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}

	return nullptr;
}

UPawnCombatComponent* UPlaygroundFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EPlaygroundValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	OutValidType = CombatComponent ? EPlaygroundValidType::Valid : EPlaygroundValidType::InValid;

	return CombatComponent;
}

bool UPlaygroundFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

float UPlaygroundFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UPlaygroundFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{
	check(InAttacker && InVictim);

	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
	OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

	const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);

	if (CrossResult.Z < 0.f)
	{
		OutAngleDifference *= -1.f;
	}

	if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f)
	{
		return PlaygroundGameplayTags::Shared_Status_HitReact_Front;
	}
	else if (OutAngleDifference < -45.f && OutAngleDifference >= -135.f)
	{
		return PlaygroundGameplayTags::Shared_Status_HitReact_Left;
	}
	else if (OutAngleDifference < -135.f || OutAngleDifference > 135.f)
	{
		return PlaygroundGameplayTags::Shared_Status_HitReact_Back;
	}
	else if (OutAngleDifference > 45.f && OutAngleDifference <= 135.f)
	{
		return PlaygroundGameplayTags::Shared_Status_HitReact_Right;
	}

	return PlaygroundGameplayTags::Shared_Status_HitReact_Front;
}

bool UPlaygroundFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
	check(InAttacker && InDefender);

	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

	//const FString DebugString = FString::Printf(TEXT("Dot Result %f %s"), DotResult, DotResult < -0.1f ? TEXT("Blocked") : TEXT("Not Blocked"));

	//Debug::Print(DebugString, DotResult < -0.1f ? FColor::Green : FColor::Red);

	return DotResult < -0.1f;
}

bool UPlaygroundFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UPlaygroundAbilitySystemComponent* SourceASC = NativeGetPlaygroundASCFromActor(InInstigator);
	UPlaygroundAbilitySystemComponent* TargetASC = NativeGetPlaygroundASCFromActor(InTargetActor);

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UPlaygroundFunctionLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, EPlaygroundCountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") EPlaygroundCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo)
{
	UWorld* World = nullptr;

	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}

	if (!World)
	{
		return;
	}


	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();

	FPlaygroundCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FPlaygroundCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);
	
	if (CountDownInput == EPlaygroundCountDownActionInput::Start)
	{
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(
				LatentInfo.CallbackTarget,
				LatentInfo.UUID,
				new FPlaygroundCountDownAction(TotalTime, UpdateInterval, OutRemainingTime, CountDownOutput, LatentInfo)
			);
		}
	}
	if (CountDownInput == EPlaygroundCountDownActionInput::Cancel)
	{
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}
}

UPlaygroundGameInstance* UPlaygroundFunctionLibrary::GetPlaygroundGameInstance(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			return World->GetGameInstance<UPlaygroundGameInstance>();
		}
	}

	return nullptr;
}

void UPlaygroundFunctionLibrary::ToggleInputMode(EPlaygroundInputMode InInputMode, const UObject* WorldContextObject)
{
	APlayerController* PlayerController = nullptr;

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		PlayerController = World->GetFirstPlayerController();
	}

	if (!PlayerController)
	{
		return;
	}

	FInputModeGameOnly GameOnlyMode;
	FInputModeUIOnly UIOnlyMode;

	switch (InInputMode)
	{
	case EPlaygroundInputMode::GameOnly:
		PlayerController->SetInputMode(GameOnlyMode);
		PlayerController->bShowMouseCursor = false;

		break;

	case EPlaygroundInputMode::UIOnly:
		PlayerController->SetInputMode(UIOnlyMode);
		PlayerController->bShowMouseCursor = true;
		break;
	default:
		break;
	}
}

void UPlaygroundFunctionLibrary::SaveCurrentGameDifficulty(EPlaygroundGameDifficulty InDifficultyToSave)
{
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(UPlaygroundSaveGame::StaticClass());

	if (UPlaygroundSaveGame* PlaygroundSaveGameObject = Cast<UPlaygroundSaveGame>(SaveGameObject))
	{
		PlaygroundSaveGameObject->SavedCurrentGameDifficulty = InDifficultyToSave;

		const bool bWasSaved = UGameplayStatics::SaveGameToSlot(PlaygroundSaveGameObject, PlaygroundGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);

		Debug::Print(bWasSaved ? TEXT("Difficulty Saved") : TEXT("Difficulty Not Saved"));
	}
}

bool UPlaygroundFunctionLibrary::TryLoadSavedGameDifficulty(EPlaygroundGameDifficulty& OutSavedDifficutly)
{
	if (UGameplayStatics::DoesSaveGameExist(PlaygroundGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0))
	{
		USaveGame* SaveGameObject = UGameplayStatics::LoadGameFromSlot(PlaygroundGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);

		if (UPlaygroundSaveGame* PlaygroundSaveGameObject = Cast<UPlaygroundSaveGame>(SaveGameObject))
		{
			OutSavedDifficutly  = PlaygroundSaveGameObject->SavedCurrentGameDifficulty;

			//Debug::Print(TEXT("Loading Successful"), FColor::Green);

			return true;
		}
	}

	return false;
}

void UPlaygroundFunctionLibrary::SaveCurrentLookSensitivity(float InYaw, float InPitch)
{
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(UPlaygroundSaveGame::StaticClass());

	if (UPlaygroundSaveGame* PlaygroundSaveGameObject = Cast<UPlaygroundSaveGame>(SaveGameObject))
	{
		PlaygroundSaveGameObject->SavedLookSensitivityYaw = InYaw;
		PlaygroundSaveGameObject->SavedLookSensitivityPitch = InPitch;

		const bool bWasSaved = UGameplayStatics::SaveGameToSlot(
			PlaygroundSaveGameObject,
			PlaygroundGameplayTags::GameData_SaveGame_Slot_2.GetTag().ToString(),
			0
		);

		Debug::Print(bWasSaved ? TEXT("Look Sensitivity Saved") : TEXT("Look Sensitivity Not Saved"));
	}
}

bool UPlaygroundFunctionLibrary::TryLoadSavedLookSensitivity(float& OutYaw, float& OutPitch)
{
	if (UGameplayStatics::DoesSaveGameExist(PlaygroundGameplayTags::GameData_SaveGame_Slot_2.GetTag().ToString(), 0))
	{
		USaveGame* SaveGameObject = UGameplayStatics::LoadGameFromSlot(
			PlaygroundGameplayTags::GameData_SaveGame_Slot_2.GetTag().ToString(),0);

		if (UPlaygroundSaveGame* PlaygroundSaveGameObject = Cast<UPlaygroundSaveGame>(SaveGameObject))
		{
			OutYaw = PlaygroundSaveGameObject->SavedLookSensitivityYaw;
			OutPitch = PlaygroundSaveGameObject->SavedLookSensitivityPitch;

			return true;
		}
	}

	return false;
}

void UPlaygroundFunctionLibrary::SaveInventory(const TArray<FInventorySlotInfo>& Slots)
{
	UPlaygroundSaveGame* SaveGameObject =
		Cast<UPlaygroundSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UPlaygroundSaveGame::StaticClass())
		);

	if (!SaveGameObject) return;

	SaveGameObject->SavedInventorySlots = Slots;

	UGameplayStatics::SaveGameToSlot(
		SaveGameObject,
		TEXT("InventorySlot"),
		0
	);
}

bool UPlaygroundFunctionLibrary::TryLoadInventory(TArray<FInventorySlotInfo>& OutSlots)
{
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("InventorySlot"), 0)) return false;

	UPlaygroundSaveGame* SaveGameObject =
		Cast<UPlaygroundSaveGame>(
			UGameplayStatics::LoadGameFromSlot(TEXT("InventorySlot"), 0)
		);

	if (!SaveGameObject) return false;

	OutSlots = SaveGameObject->SavedInventorySlots;

	return true;
}

