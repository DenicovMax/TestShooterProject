// Fill out your copyright notice in the Description page of Project Settings.


#include "SP/Public/Weapon/SPGrenade.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


ASPGrenade::ASPGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	if(GrenadeMesh)
	{
		SetRootComponent(GrenadeMesh);
		GrenadeMesh->OnComponentBeginOverlap.AddDynamic(this, &ASPGrenade::OnBeginOverlap);
	}

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	bReplicates		  = true;
}

void ASPGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void ASPGrenade::StartFire()
{
	if(MovementComponent)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerToExplosion, this, &ASPGrenade::Detonation,
											   Info.TimeToExplosion);
	}
}

void ASPGrenade::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep,
								   const FHitResult& SweepResult)
{
	if(HasAuthority() && OtherActor != GetOwner())
	{
		Detonation();
	}
}

void ASPGrenade::Detonation()
{
	if(HasAuthority() && GetOwner())
	{
		FVector loc = GrenadeMesh->GetComponentLocation();
		UGameplayStatics::ApplyRadialDamage(this, Info.Damage, loc, Info.InnerRadius, Info.DamageType, {}, nullptr,
											GetOwner()->GetInstigatorController());
		Multi_PlayEffect();
	}
}

void ASPGrenade::Multi_PlayEffect_Implementation()
{
	if(!HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, Info.ExplosionSound, GrenadeMesh->GetComponentLocation());
		UParticleSystemComponent* ParticleSystemComponent =
			UGameplayStatics::SpawnEmitterAtLocation(this, Info.ExplosionParticle, GrenadeMesh->GetComponentLocation());
		ParticleSystemComponent->Deactivate();
	}
	Destroy();
}

