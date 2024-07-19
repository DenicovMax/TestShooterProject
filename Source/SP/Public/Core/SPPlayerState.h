// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SP_API ASPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Replicated, BlueprintGetter= GetMyKills)
	int MyKills = 0;

	UFUNCTION(BlueprintGetter)
	int GetMyKills() const { return MyKills; }

	UPROPERTY(EditAnywhere, Replicated, BlueprintGetter= GetMyDeaths)
	int MyDeaths = 0;

	UFUNCTION(BlueprintGetter)
	int GetMyDeaths() const { return MyDeaths; }
	
	UPROPERTY(EditAnywhere, Replicated, BlueprintGetter= GetIsMeDead)
	bool IsMeDead = false;

	UFUNCTION(BlueprintGetter)
	bool GetIsMeDead() const { return IsMeDead; }

	UFUNCTION(BlueprintCallable)
	void AddDead()
	{
		MyDeaths += 1;
		IsMeDead = true;
	}

	UFUNCTION(BlueprintCallable)
	void AddKill()
	{
		MyKills += 1;
	}

protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
