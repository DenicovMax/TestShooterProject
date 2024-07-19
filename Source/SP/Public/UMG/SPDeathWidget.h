// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPDeathWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class SP_API USPDeathWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;

	UButton* GetRestartButton() const { return RestartButton; }

protected:
	
	UFUNCTION()
	void OnExitButtonDown();

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;
};
