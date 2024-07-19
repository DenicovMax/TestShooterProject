// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SPHealthComponent.h"
#include "Net/UnrealNetwork.h"


USPHealthComponent::USPHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USPHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void USPHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USPHealthComponent, CurrentHealth);
}

void USPHealthComponent::ChangeHealth(float DamageValue)
{
	CurrentHealth -= DamageValue;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Current HP %f"), CurrentHealth));
	if(CurrentHealth <= 0.0f)
	{
		Death();
	}
}

void USPHealthComponent::OnRep_CurrentHealth()
{
	OnHealthChangeSignature.Broadcast(CurrentHealth);
}

void USPHealthComponent::Death()
{
	IsDead = true;
	if(OnDeathSignature.IsBound())
	{
		OnDeathSignature.Broadcast();
	}
}

