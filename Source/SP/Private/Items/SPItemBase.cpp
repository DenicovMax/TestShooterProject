// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SPItemBase.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASPItemBase::ASPItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshPreview");
	if(Mesh)
	{
		SetRootComponent(Mesh);
		Mesh->OnComponentBeginOverlap.AddDynamic(this, &ASPItemBase::OnComponentBeginOverlap);
	}

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	if(ParticleSystem)
	{
		ParticleSystem->SetupAttachment(Mesh);
	}
}

void ASPItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASPItemBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
												   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
												   bool bBFromSweep, const FHitResult& SweepResult)
{
	Destroy();
}
