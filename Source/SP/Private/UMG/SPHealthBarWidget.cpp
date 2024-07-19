// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/SPHealthBarWidget.h"
#include "Characters/SPMainCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/SPHealthComponent.h"
#include "Kismet/KismetMathLibrary.h"


void USPHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HealthBar->SetPercent(1.f);

	if(ASPMainCharacter* Char = Cast<ASPMainCharacter>(GetOwningPlayerPawn()))
	{
		if(Char->GetHealthComponent())
		{
			MaxHealth = Char->GetHealthComponent()->GetMaxHealth();
			Char->GetHealthComponent()->OnHealthChangeSignature.AddUObject(this, &USPHealthBarWidget::SetHealth);
		}
	}
}

void USPHealthBarWidget::SetHealth(float NewHealth) const
{
	HealthBar->SetPercent(UKismetMathLibrary::Clamp(NewHealth, 0.f, MaxHealth) / MaxHealth);
}