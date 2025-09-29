// Copyright 2025. Jinsol Co. All rights reserved


#include "PlaygroundFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/PawnCombatInterface.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"

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

void UPlaygroundFunctionLibrary::RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
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
