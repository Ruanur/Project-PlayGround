// Copyright 2025. Jinsol Co. All rights reserved


#include "Items/Fragment/Playground_ItemFragment.h"
#include "AbilitySystem/PlaygroundAbilitySystemComponent.h"
#include "Widgets/Composite/Playground_CompositeBase.h"
#include "Widgets/Composite/Playground_LeafImage.h"
#include "Widgets/Composite/Playground_LeafText.h"
#include "Widgets/Composite/Playground_LeafLabeledValue.h"
#include "Widgets/Composite/Playground_RarityBorderLeaf.h"
#include "AbilitySystem/PlaygroundAttributeSet.h"
#include "GameplayEffect.h"
#include "EquipmentManagement/EqiupActor/Playground_EquipActor.h"
#include "GameFramework/PlayerState.h"
#include "Items/Drops/Manifest/Playground_ItemManifest.h"

#include "PlaygroundDebugHelper.h"

void FPlayground_InventoryItemFragment::Assimilate(UPlayground_CompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	Composite->PG_Expand();
}

void FPlayground_TextFragment::Assimilate(UPlayground_CompositeBase* Composite) const
{
	FPlayground_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UPlayground_LeafText* LeafText = Cast<UPlayground_LeafText>(Composite);
	if (!IsValid(LeafText)) return;

	LeafText->PG_SetText(FragmentText);
}

bool FPlayground_InventoryItemFragment::MatchesWidgetTag(const UPlayground_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

// Get a Stats component from the PC or the PC->GetPawn()
// or get the Ability System Component and apply a Gameplay Effect
// or call an interface function for Healing()
void FPlayground_HealthPotionFragment::OnConsume(APlayerController* PC, UPlaygroundAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	if (!PC) return;

	APawn* Pawn = PC->GetPawn();
	if (!Pawn) return;

	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC) return;

	const UGameplayEffect* EffectCDO =
		ConsumableGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	ASC->ApplyGameplayEffectToSelf(
		EffectCDO,
		1,
		ASC->MakeEffectContext()
	);
	
	Debug::Print(TEXT("Health Potion consumed %f"), FColor::Green);
}

void FPlayground_ManaPotionFragment::OnConsume(APlayerController* PC, UPlaygroundAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	// Replenish mana however you wish

	Debug::Print(TEXT("Mana Potion consumed %f"), FColor::Blue, ManaAmount);
}

void FPlayground_ImageFragment::Assimilate(UPlayground_CompositeBase* Composite) const
{
	FPlayground_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UPlayground_LeafImage* Image = Cast<UPlayground_LeafImage>(Composite);
	if (!IsValid(Image)) return;

	Image->SetImage(Icon);
	Image->SetBoxSize(IconDimensions);
	Image->SetImageSize(IconDimensions);

}

void FPlayground_LabeledNumberFragment::Assimilate(UPlayground_CompositeBase* Composite) const
{
	FPlayground_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UPlayground_LeafLabeledValue* LabeledValue = Cast<UPlayground_LeafLabeledValue>(Composite);
	if (!IsValid(LabeledValue)) return;

	LabeledValue->SetText_Label(Text_Label, bCollapseLabel);

	FNumberFormattingOptions Options;
	Options.MinimumFractionalDigits = MinFractionalDigits;
	Options.MaximumFractionalDigits = MaxFractionalDigits;

	LabeledValue->SetText_Value(FText::AsNumber(Value, &Options), bCollapseValue);
	
}

void FPlayground_ItemRarity::Assimilate(UPlayground_CompositeBase* Composite) const
{
	FPlayground_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UPlayground_RarityBorderLeaf* RarityLeaf = Cast<UPlayground_RarityBorderLeaf>(Composite);
	if (!IsValid(RarityLeaf)) return;

	RarityLeaf->SetBorderByRarity(Rarity);
}

void FPlayground_LabeledNumberFragment::Manifest()
{
	FPlayground_InventoryItemFragment::Manifest();

	//if (!SourceGameplayEffect) return;

	//const UGameplayEffect* GECDO = SourceGameplayEffect->GetDefaultObject<UGameplayEffect>();

	//const FGameplayModifierInfo& Modifier = GECDO->Modifiers[0];

	//float OutValue = 0.f;

	//if (Modifier.ModifierMagnitude.AttemptCalculateMagnitude(
	//	FGameplayEffectSpec(),
	//	OutValue))
	//{
	//	Value = OutValue;
	//}

	if (bRandomizeOnManifest)
	{
		Value = FMath::FRandRange(Min, Max);
	}
 
	bRandomizeOnManifest = false;
}


void FPlayground_StrengthModifier::OnEquip(APlayerController* PC, float RarityMultiplier)
{
	if (!PC || !EquipmentEffectClass) return;

	APawn* Pawn = PC->GetPawn();
	if (!Pawn) return;

	// 서버에만 적용
	if (!Pawn->HasAuthority()) return;

	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC && PC->PlayerState)
	{
		ASC = PC->PlayerState->FindComponentByClass<UAbilitySystemComponent>();
	}
	if (!ASC) return;

	if (ActiveEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
		ActiveEffectHandle.Invalidate();
	}

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(Pawn);

	const float BaseValue = GetValue();
	const float FinalValue = BaseValue * RarityMultiplier;

	const float LevelToApply = 1.f;
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EquipmentEffectClass, LevelToApply, Context);
	if (!SpecHandle.IsValid()) return;

	const FGameplayTag StrengthTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Stats.Strength"));
	SpecHandle.Data->SetSetByCallerMagnitude(StrengthTag, FinalValue);

	ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	Debug::Print(FString::Printf(TEXT("Strength increased by : %.1f"), FinalValue), FColor::Green);
	Debug::Print(FString::Printf(
		TEXT("BaseValue=%.2f, RarityMultiplier=%.2f, FinalValue=%.2f"),
		BaseValue, RarityMultiplier, FinalValue),
		FColor::Yellow
	);
}

void FPlayground_StrengthModifier::OnUnequip(APlayerController* PC)
{
	if (!PC) return;

	APawn* Pawn = PC->GetPawn();
	if (!Pawn) return;

	if (!Pawn->HasAuthority()) return;

	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();

	if (!ASC && PC->PlayerState)
	{
		ASC = PC->PlayerState->FindComponentByClass<UAbilitySystemComponent>();
	}

	if (!ASC) return;

	if (ActiveEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
		ActiveEffectHandle.Invalidate();
	}

	Debug::Print(TEXT("Strength decreased by : 15.0"), FColor::Red);
}


void FPlayground_EquipmentFragment::OnEquip(APlayerController* PC, float RarityMultiplier)
{
	if (bEquipped) return;
	bEquipped = true;

	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnEquip(PC, RarityMultiplier);
	}
}

void FPlayground_EquipmentFragment::OnUnequip(APlayerController* PC)
{
	if (!bEquipped) return;
	bEquipped = false;

	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnUnequip(PC);
	}
}

void FPlayground_EquipmentFragment::Assimilate(UPlayground_CompositeBase* Composite) const
{
	FPlayground_InventoryItemFragment::Assimilate(Composite);
	for (const auto& Modifier : EquipModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FPlayground_EquipmentFragment::Manifest()
{
	FPlayground_InventoryItemFragment::Manifest();

	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.Manifest();
	}
}

APlayground_EquipActor* FPlayground_EquipmentFragment::SpawnAttachedActor(USkeletalMeshComponent* AttachMesh) const
{
	//if (!IsValid(EquipActorClass) || !IsValid(AttachMesh)) return nullptr;

	//APlayground_EquipActor* SpawnedActor = AttachMesh->GetWorld()->SpawnActor<APlayground_EquipActor>(EquipActorClass);
	//SpawnedActor->AttachToComponent(AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketAttachPoint);

	//return SpawnedActor;

	return nullptr;
}

void FPlayground_EquipmentFragment::DestroyAttachedActor() const
{
	if (EquippedActor.IsValid())
	{
		EquippedActor->Destroy();
	}
}

void FPlayground_EquipmentFragment::SetEquippedActor(APlayground_EquipActor* EquipActor)
{
	// 임시 코드 : nullptr 호출하도록 했기에 return 반환
	if (!IsValid(EquipActor)) return;

	EquippedActor = EquipActor;
}


