// Copyright 2025. Jinsol Co. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaygroundProjectileBase.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;

UENUM(BlueprintType)
enum class EProjectileDamagePolicy : uint8
{
	OnHit,
	OnBeginOverlap
};

UCLASS()
class PROJECT_V_API APlaygroundProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaygroundProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UBoxComponent* ProjectileCollisionBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UNiagaraComponent* ProjectileNiagaraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	EProjectileDamagePolicy ProjectileDamagePolicy = EProjectileDamagePolicy::OnHit;

	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Spawn Projectile Hit FX"))
	void BP_OnSpawnProjectileHitFX(const FVector& HitLocation);
};
