// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPGrenade.generated.h"

class UProjectileMovementComponent;

USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Damage = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InnerRadius = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float OuterRadius = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageFalloff = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeToExplosion = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* ExplosionParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType;
};

UCLASS()
class SP_API ASPGrenade : public AActor
{
	GENERATED_BODY()

public:
	
	ASPGrenade();

protected:
	
	virtual void BeginPlay() override;

public:
	
	void StartFire();

protected:
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);

	void Detonation();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayEffect();

protected:
	
	FTimerHandle TimerToExplosion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* GrenadeMesh;

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FWeaponInfo Info;
};
