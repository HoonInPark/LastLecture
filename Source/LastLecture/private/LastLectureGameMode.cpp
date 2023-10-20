// Copyright Epic Games, Inc. All Rights Reserved.

#include "LastLectureGameMode.h"
#include "LastLectureCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALastLectureGameMode::ALastLectureGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
