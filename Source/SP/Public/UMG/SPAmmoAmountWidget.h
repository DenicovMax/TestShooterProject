// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPAmmoAmountWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class SP_API USPAmmoAmountWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;

protected:
	
	UFUNCTION()
	void OnChangeCurrentAmount(int32 NewCurrentAmount) const;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* CurrentAmmoText;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* MaxAmmoText;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(GetOptions="GetWeaponOptions"))
	FString WeaponType;

	UFUNCTION()
	TArray<FString> GetWeaponOptions() const
	{
		return { "Grenade", "Rifle" };
	}
};
