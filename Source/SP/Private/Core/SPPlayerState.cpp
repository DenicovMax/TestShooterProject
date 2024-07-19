// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SPPlayerState.h"

#include "Net/UnrealNetwork.h"

void ASPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPPlayerState, MyKills);
	DOREPLIFETIME(ASPPlayerState, MyDeaths);
}
