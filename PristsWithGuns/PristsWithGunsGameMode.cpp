// Copyright Epic Games, Inc. All Rights Reserved.

#include "PristsWithGunsGameMode.h"
#include "PristsWithGunsCharacter.h"
#include "UObject/ConstructorHelpers.h"

APristsWithGunsGameMode::APristsWithGunsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
