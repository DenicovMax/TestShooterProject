// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SPPlayerController.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Core/SPPlayerState.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UMG/SPDeathWidget.h"

class APlayerStart;

ASPPlayerController::ASPPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
	EnableInput(this);
}

void ASPPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ASPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ASPPlayerController::OnDeath()
{
	Client_OnDeath();
	IsDead = true;
}

void ASPPlayerController::Client_OnDeath_Implementation()
{
	if(DeathMenuWidgetClass)
	{
		DeathWidget = CreateWidget<USPDeathWidget>(this, DeathMenuWidgetClass);
		DeathWidget->AddToViewport(99);
		DeathWidget->GetRestartButton()->OnPressed.AddDynamic(this, &ASPPlayerController::Restart);
		SetShowMouseCursor(true);
		GetPlayerState<ASPPlayerState>()->IsMeDead = true;
		SetInputMode(FInputModeUIOnly());
		
		//GetPlayerState<ASPPlayerState>()->AddDead();
	}
}

void ASPPlayerController::Restart()
{
	if(DeathWidget)
	{
		DeathWidget->SetVisibility(ESlateVisibility::Hidden);
		DeathWidget->RemoveFromParent();
	}
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());

	//GetPlayerState<ASPPlayerState>()->IsMeDead = false;
	//IsDead = false;
	
	Server_RestartPlayer();
}

void ASPPlayerController::Server_RestartPlayer_Implementation()
{
	if(HasAuthority())
	{
		FVector NewLocationToSpawn;
		FRotator NewRotationToSpawn;
		TArray<APlayerStart*> PlayersStart;
		for(TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
		{
			APlayerStart* Start = *It;
			PlayersStart.Add(Start);
		}

		// Selecting a random spawn point
		int32 RandPosition = UKismetMathLibrary::RandomInteger(PlayersStart.Num() - 1);
		if(RandPosition <= PlayersStart.Num())
		{
			if(PlayersStart[RandPosition])
			{
				NewLocationToSpawn = PlayersStart[RandPosition]->GetActorLocation();
				NewRotationToSpawn = PlayersStart[RandPosition]->GetActorRotation();
			}
		}

		if(GetWorld() && UGameplayStatics::GetGameMode(this))
		{
			FActorSpawnParameters param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			APawn* mCharacter = GetWorld()->SpawnActor<APawn>(UGameplayStatics::GetGameMode(this)->DefaultPawnClass,
															  NewLocationToSpawn, NewRotationToSpawn, param);
			Possess(mCharacter);
		}

		GetPlayerState<ASPPlayerState>()->IsMeDead = false;
	}
}