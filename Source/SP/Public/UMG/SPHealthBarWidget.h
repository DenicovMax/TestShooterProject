// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPHealthBarWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class SP_API USPHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;

	void SetHealth(float NewHealth) const;

protected:
	
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UProgressBar* HealthBar;
};
