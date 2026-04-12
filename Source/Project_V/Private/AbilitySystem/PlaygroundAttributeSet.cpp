// Copyright 2025. Jinsol Co. All rights reserved


#include "AbilitySystem/PlaygroundAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "PlaygroundFunctionLibrary.h"
#include "PlaygroundGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Controllers/PlayGroundPlayerController.h"

#include "PlaygroundDebugHelper.h"

UPlaygroundAttributeSet::UPlaygroundAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
	InitBaseDamage(1.f);
	InitBonusDamage(1.f);
}

void UPlaygroundAttributeSet::BroadcastHealthUI() const
{
	if (!CachedPawnUIInterface.IsValid()) return;

	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	UPlayerUIComponent* PlayerUIComponent = CachedPawnUIInterface->GetPlayerUIComponent();

	const float MaxHealthValue = GetMaxHealth();
	const float HealthPercent = MaxHealthValue > 0.f ? GetCurrentHealth() / MaxHealthValue : 0.f;

	if (PawnUIComponent)
	{
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(HealthPercent);
	}

	if (PlayerUIComponent)
	{
		PlayerUIComponent->OnHealthValuesChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());
	}
}

void UPlaygroundAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);

	if (!CachedPawnUIInterface.IsValid())
	{
		if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
		{
			if (AActor* AvatarActor = ASC->GetAvatarActor())
			{
				CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(AvatarActor);
			}
		}
	}

	// 장비 / 지속 GameEffect 등으로 MaxHealth가 바뀌는 경우를 여기서 잡음
	if (Attribute == GetMaxHealthAttribute())
	{
		const float ClampedHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		if (!FMath::IsNearlyEqual(GetCurrentHealth(), ClampedHealth))
		{
			SetCurrentHealth(ClampedHealth);
		}

		BroadcastHealthUI();
	}
}



void UPlaygroundAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}

	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();

	checkf(PawnUIComponent, TEXT("Couldn't Extrac a PawnUIComponent from %s"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

	UPlayerUIComponent* PlayerUIComponent = CachedPawnUIInterface->GetPlayerUIComponent();

	// ----------------------
	// CurrentHealth
	// ----------------------
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		BroadcastHealthUI();
		//// 퍼센트 Health
		//PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());

		//if (PlayerUIComponent)
		//{
		//	PlayerUIComponent->OnHealthValuesChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());
		//}
	}

	// ----------------------
	// MaxHealth
	// ----------------------
	//if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	//{
	//	const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
	//	SetCurrentHealth(NewCurrentHealth);

	//	PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

	//	if (PlayerUIComponent)
	//	{
	//		PlayerUIComponent->OnHealthValuesChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());
	//	}
	//}


	// ----------------------
	// CurrentRage
	// ----------------------
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());

		SetCurrentRage(NewCurrentRage);

		if (GetCurrentRage() == GetMaxRage())
		{
			UPlaygroundFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), PlaygroundGameplayTags::Player_Status_Rage_Full);
		}
		else if(GetCurrentRage() == 0.f)
		{
			UPlaygroundFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), PlaygroundGameplayTags::Player_Status_Rage_None);
		}
		else
		{
			UPlaygroundFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), PlaygroundGameplayTags::Player_Status_Rage_Full);
			UPlaygroundFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), PlaygroundGameplayTags::Player_Status_Rage_None);
		}

		if (PlayerUIComponent)
		{
			PlayerUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
			PlayerUIComponent->OnRageValuesChanged.Broadcast(GetCurrentRage(), GetMaxRage());
		}
	}

	// ----------------------
	// MaxRage
	// ----------------------
	if (Data.EvaluatedData.Attribute == GetMaxRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentRage);

		if (PlayerUIComponent)
		{
			PlayerUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
			PlayerUIComponent->OnRageValuesChanged.Broadcast(GetCurrentRage(), GetMaxRage());
		}
	}

	// ----------------------
	// AttackPower / DefensePower
	// ----------------------
	if (Data.EvaluatedData.Attribute == GetAttackPowerAttribute())
	{
		if (PlayerUIComponent)
		{
			PlayerUIComponent->OnAttackPowerChanged.Broadcast(GetAttackPower());
		}
	}

	if (Data.EvaluatedData.Attribute == GetDefensePowerAttribute())
	{
		if (PlayerUIComponent)
		{
			PlayerUIComponent->OnDefensePowerChanged.Broadcast(GetDefensePower());
		}
	}

	// ----------------------
	// BaseDamage
	// ----------------------
	if (Data.EvaluatedData.Attribute == GetBaseDamageAttribute())
	{
		 if (PlayerUIComponent)
		 {
			 PlayerUIComponent->OnBaseDamageChanged.Broadcast(GetBaseDamage());
		 }
	}

	// ----------------------
	// DamageTaken
	// ----------------------
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		const FString DebugString = FString::Printf(
			TEXT("Old Health : %f, Damage Done : %f, NewCurrentHealth : %f"),
			OldHealth,
			DamageDone,
			NewCurrentHealth
		);

		Debug::Print(DebugString, FColor::Green);

		//TODO: Notify the UI
		//이벤트 전달, 브로드캐스트 - 이 항목이 없으면 체력 변화에도 체력바가 동기화되지 않음
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		if (DamageDone > 0.f)
		{
			ShowFloatingText(Data, DamageDone);
		}
		
		//TODO: Handle Character Death 
		if (GetCurrentHealth() == 0.f)
		{
			UPlaygroundFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), PlaygroundGameplayTags::Shared_Status_Dead);
		}
	}
}

void UPlaygroundAttributeSet::ShowFloatingText(const FGameplayEffectModCallbackData& Data, float Damage) const
{
	if (Damage <= 0.f) return;

	AActor* TargetActor = Data.Target.GetAvatarActor();
	if (!IsValid(TargetActor)) return;

	UAbilitySystemComponent* SourceASC = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	if (!IsValid(SourceActor)) return;

	if (SourceActor == TargetActor) return;

	APawn* SourcePawn = Cast<APawn>(SourceActor);
	if (!SourcePawn) return;

	APlayGroundPlayerController* PC = Cast<APlayGroundPlayerController>(SourcePawn->GetController());
	if (!PC) return;

	PC->ShowDamageNumber(Damage, TargetActor);
}

