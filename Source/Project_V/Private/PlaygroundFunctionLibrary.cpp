// Copyright 2025. Jinsol Co. All rights reserved


#include "PlaygroundFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"

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
