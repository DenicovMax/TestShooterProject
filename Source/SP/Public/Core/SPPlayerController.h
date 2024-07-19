// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPPlayerController.generated.h"

class USPDeathWidget;
/**
 * 
 */
UCLASS()
class SP_API ASPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	ASPPlayerController();

protected:
	
	virtual void BeginPlay() override;

	virtual void PlayerTick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	UFUNCTION()
	void Restart();

	UFUNCTION(Server, Reliable)
	void Server_RestartPlayer();

public:
	
	void OnDeath();

	UFUNCTION(Client, Reliable)
	void Client_OnDeath();

	UPROPERTY()
	bool IsDead = false;

	UFUNCTION(BlueprintCallable)
	bool GetIsDead() const
	{
		return IsDead;
	}
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> DeathMenuWidgetClass;

	UPROPERTY()
	USPDeathWidget* DeathWidget;
};
