// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPBaseCharacter.h"
#include "Weapon/SPGrenade.h"
#include "SPMainCharacter.generated.h"



class USPManagerWidget;
class USPHealthComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamage, float /*Damage*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFireGrenade, int32 /*CurrentAmount*/)

UCLASS()
class SP_API ASPMainCharacter : public ASPBaseCharacter
{
	GENERATED_BODY()

public:
	
	ASPMainCharacter();

protected:
	
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void GrenadeLaunch();

	UFUNCTION(Server, Reliable)
	void Server_GrenadeLaunch();

	void StartRifleFire();

	UFUNCTION(Server, Reliable)
	void Server_StartRifleFire();

	void DeathCharacter();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_Death();

	void CreateHUD();

	void RemoveHUD();

	UFUNCTION()
	void OnBeginOverlapCapsule(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep,
							   const FHitResult& SweepResult);

	UFUNCTION()
	void TakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
						  FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser);
						  
//	UFUNCTION()
//	void TakeAnyDamage(AActor* DamagedActor, float Damage, const  UDamageType* DamageType,
//	 AController* InstigatedBy, AActor* DamageCauser );

	UFUNCTION()
	void OnRep_CurrentAmountGrenades();

	UFUNCTION()
	void OnRep_CurrentAmountRifleAmmo();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* StartRifleFireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrenadeLaunchAction;

public:
	
	USPHealthComponent* GetHealthComponent() const { return HealthComponent; }

	int32 GetCurrentGrenades() const { return CurrentGrenades; }
	
	int32 GetMaxGrenades() const { return MaxGrenades; }

	int32 GetCurrentRifleAmmo() const { return CurrentRifleAmmo; }

	int32 GetMaxRifleAmmo() const { return MaxRifleAmmo; }

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USPHealthComponent* HealthComponent;

	UPROPERTY()
	USPManagerWidget* MainHUD;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDClassWidget;

	UPROPERTY(Replicated)
	TSubclassOf<ASPGrenade> GrenadeInHand;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentAmountGrenades)
	int32 CurrentGrenades = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxGrenades = 4;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentAmountRifleAmmo)
	int32 CurrentRifleAmmo = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxRifleAmmo = 90;

public:
	
	FOnDamage OnDamageSignature;

	FOnFireGrenade OnFireGrenadeSignature;
	FOnFireGrenade OnFireRifleSignature;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FWeaponInfo RifleInfo;
};
