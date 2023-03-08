// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyProjectPlayerController.h"
#include "MyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyProjectGameMode::AMyProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMyProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
//	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/2DCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
