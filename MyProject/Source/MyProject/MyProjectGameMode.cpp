// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyProjectPlayerController.h"
#include "UserInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "MyProject2DCharacter.h"
#include "Kismet/GameplayStatics.h"

AMyProjectGameMode::AMyProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMyProjectPlayerController::StaticClass();


	// set default pawn class to our Blueprinted character
//	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/BP_Yul"));
    UE_LOG(LogTemp,Warning,TEXT("gamemode before if"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
        UE_LOG(LogTemp,Warning,TEXT("gamemode after  if"));
	}
}


void AMyProjectGameMode::BeginPlay()
{
    AMyProject2DCharacter* MyCharacter = Cast<AMyProject2DCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    if(IsValid(Interface))
    {
        UserInterface = Cast<UUserInterface>(CreateWidget(GetWorld(),Interface));
        if(UserInterface != nullptr)
        {
            UserInterface->AddToViewport();
            UserInterface->SetOwner(MyCharacter);
        }
    }
    
    
}
