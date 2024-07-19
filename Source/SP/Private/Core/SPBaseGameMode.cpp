// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/SPBaseGameMode.h"
#include "Characters/SPBaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASPBaseGameMode::ASPBaseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
