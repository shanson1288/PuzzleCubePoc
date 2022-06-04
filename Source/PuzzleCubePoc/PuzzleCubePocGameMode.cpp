// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuzzleCubePocGameMode.h"
#include "PuzzleCubePocCharacter.h"
#include "UObject/ConstructorHelpers.h"

APuzzleCubePocGameMode::APuzzleCubePocGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
