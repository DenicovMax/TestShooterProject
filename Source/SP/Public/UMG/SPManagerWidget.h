// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPAmmoAmountWidget.h"
#include "SPHealthBarWidget.h"
#include "Blueprint/UserWidget.h"
#include "SPManagerWidget.generated.h"

/**
 * 
 */
UCLASS()
class SP_API USPManagerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	USPHealthBarWidget* HealthWidget;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	USPAmmoAmountWidget* GrenadeWidget;
};
