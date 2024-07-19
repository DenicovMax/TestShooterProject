// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/SPAmmoAmountWidget.h"

#include "Characters/SPMainCharacter.h"
#include "Components/TextBlock.h"


void USPAmmoAmountWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(ASPMainCharacter* Char = Cast<ASPMainCharacter>(GetOwningPlayerPawn()))
	{
		
		if(WeaponType == "Grenade")
		{
			Char->OnFireGrenadeSignature.AddUObject(this, &USPAmmoAmountWidget::OnChangeCurrentAmount);
			CurrentAmmoText->SetText(FText::FromString(FString::FromInt(Char->GetCurrentGrenades())));
			MaxAmmoText->SetText(FText::FromString(FString::FromInt(Char->GetMaxGrenades())));
		}
		if(WeaponType == "Rifle")
		{
			Char->OnFireRifleSignature.AddUObject(this, &USPAmmoAmountWidget::OnChangeCurrentAmount);
			CurrentAmmoText->SetText(FText::FromString(FString::FromInt(Char->GetCurrentRifleAmmo())));
			MaxAmmoText->SetText(FText::FromString(FString::FromInt(Char->GetMaxRifleAmmo())));
		}
	}
}

void USPAmmoAmountWidget::OnChangeCurrentAmount(int32 NewCurrentAmount) const
{
	CurrentAmmoText->SetText(FText::FromString(FString::FromInt(NewCurrentAmount)));
}