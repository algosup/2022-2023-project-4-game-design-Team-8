// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyProjectPlayerController.h"
#include "UserInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "MyProject2DCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h" 
#include "TimerManager.h"
#include "EnnemyBase.h"


AMyProjectGameMode::AMyProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMyProjectPlayerController::StaticClass();


	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/2DCharacter"));
    //static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/BP_Yul"));
    static ConstructorHelpers::FClassFinder<APawn> EnnemyBase(TEXT("/Game/TopDownCPP/Blueprints/MyEnnemyBase"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    static ConstructorHelpers::FObjectFinder<USoundCue> Cue(TEXT("SoundCue'/Game/2DSideScroller/Test.Test'"));
    if (Cue.Object != nullptr)
    {
        UGameplayStatics::PlaySound2D(GetWorld(),Cue.Object,1.f, 1.f, 1.f);
    }
    if (EnnemyBase.Class != nullptr)
    {
        Ennemy = EnnemyBase.Class;
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
    if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandler))
    {
        GetWorldTimerManager().SetTimer(TimerHandler, this, &AMyProjectGameMode::SpawnEnnemies,5.f,true);
    }
    
    
}

void AMyProjectGameMode::SpawnEnnemies()
{
    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    AEnnemyBase* SpawnedActor = GetWorld()->SpawnActor<AEnnemyBase>(Ennemy,FVector(-700.f,-1192.f,160.f),FRotator(0.f,0.f,0.f), ActorSpawnParams);
}
