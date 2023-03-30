// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"


#include "MyProject2DCharacter.h"
#include "EnnemyBase.h"
#include "MyProjectPlayerController.h"
#include "UserInterface.h"
#include "PickableWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h" 
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

#include "PaperTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileLayer.h"
#include "PaperTileMap.h"
#include "SubMachineGun.h"

AMyProjectGameMode::AMyProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMyProjectPlayerController::StaticClass();


	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/2DCharacter"));
    //static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/BP_Yul"));
    static ConstructorHelpers::FClassFinder<APawn> EnnemyBase(TEXT("/Game/TopDownCPP/Blueprints/MyEnnemyBase"));
    static ConstructorHelpers::FClassFinder<APickableWeapon> PickableWeaponBase(TEXT("/Game/TopDownCPP/Blueprints/MyPickableWeapon"));
    
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    static ConstructorHelpers::FObjectFinder<USoundCue> Cue(TEXT("SoundCue'/Game/2DSideScroller/Test.Test'"));
    if (Cue.Object != nullptr)
    {
        //UGameplayStatics::PlaySound2D(GetWorld(),Cue.Object,1.f, 1.f, 1.f);
    }

    if (EnnemyBase.Class != nullptr)
    {
        Ennemy = EnnemyBase.Class;
    }
    if (PickableWeaponBase.Class != nullptr)
    {
        PickableWeapon = PickableWeaponBase.Class;
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
    /*if (APickableWeapon* Weapon = GetWorld()->SpawnActor<APickableWeapon>(PickableWeapon))
    {
        Weapon->SetActorRelativeLocation(FVector(300.f,200.f,33.f));
        UClass* SubMachineClass = ASubMachineGun::StaticClass();
        Weapon->WeaponClass = SubMachineClass;
    }*/
}

void AMyProjectGameMode::DropWeapon(ARangedWeapon* RangedWeapon,FVector PickedWeaponLocation)
{
    if (APickableWeapon* Weapon = GetWorld()->SpawnActor<APickableWeapon>(PickableWeapon))
    {
        Weapon->SetActorRelativeLocation(PickedWeaponLocation);
        Weapon->WeaponClass = RangedWeapon->GetClass();
        Weapon->InitWeapon();
        RangedWeapon->Destroy();
    }
}
//void AMyProjectGameMode::DropWeapon(ARangedWeapon* RangedWeapon,FVector PickedWeaponLocation)
//{
//    ARangedWeapon* Weapon = GetWorld()->SpawnActor<ARangedWeapon>(RangedWeapon->GetClass(),PickedWeaponLocation,FRotator::ZeroRotator);
//}

void AMyProjectGameMode::OpenDoor(FVector PlayerPosition,APaperTileMapActor* Tile,FVector PlayerLastInput)
{
    int TileX = 0;
    int TileY = 0;
    int Height = 0;
    int Width = 0;
    int NumLayers = 0;
    int DoorsClosed = 1;
    int UpperDoorTileY = 0;
    int UpperDoorTileX = 0;
    
    PlayerPosition.Z = -PlayerPosition.Y;
    
    Tile->GetRenderComponent()->GetMapSize(Width,Height,NumLayers);
    FPaperTileInfo* TileInfo = new FPaperTileInfo();
    FPaperTileInfo* UpperDoorTileInfo = new FPaperTileInfo();
    
    Tile->GetRenderComponent()->TileMap->GetTileCoordinatesFromLocalSpacePosition(PlayerPosition,TileX,TileY);

    if(abs(PlayerLastInput.Y) > abs(PlayerLastInput.X))
    {
        TileY += PlayerLastInput.Y;
        UpperDoorTileY = PlayerLastInput.Y + TileY;
        UpperDoorTileX = TileX;
    }
    else if(abs(PlayerLastInput.X) > abs(PlayerLastInput.Y))
    {
        TileX += PlayerLastInput.X;
        UpperDoorTileX = PlayerLastInput.X + TileX;
        UpperDoorTileY = TileY;
    }
    else {return;}
    
    TileInfo->TileSet = Tile->GetRenderComponent()->GetTile(TileX,TileY,DoorsClosed).TileSet;
    TileInfo->PackedTileIndex = Tile->GetRenderComponent()->GetTile(TileX,TileY,DoorsClosed).PackedTileIndex;
    TileInfo->PackedTileIndex += 1;
    
    UpperDoorTileInfo->TileSet = Tile->GetRenderComponent()->GetTile(UpperDoorTileX,UpperDoorTileY,DoorsClosed).TileSet;
    UpperDoorTileInfo->PackedTileIndex = Tile->GetRenderComponent()->GetTile(UpperDoorTileX,UpperDoorTileY,DoorsClosed).PackedTileIndex;
    UpperDoorTileInfo->PackedTileIndex += 1;
    
    Tile->GetRenderComponent()->SetTile(TileX,TileY,DoorsClosed,*TileInfo);
    Tile->GetRenderComponent()->SetTile(UpperDoorTileX,UpperDoorTileY,DoorsClosed,*UpperDoorTileInfo);
}

void AMyProjectGameMode::SpawnEnnemies()
{
    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    AEnnemyBase* SpawnedActor = GetWorld()->SpawnActor<AEnnemyBase>(Ennemy,FVector(-480.f,-1825.f,33.21f),FRotator(0.f,0.f,0.f), ActorSpawnParams);
    AEnnemyAIController* PlayerAI = GetWorld()->SpawnActor<AEnnemyAIController>(MyAIControllerClass);
    PlayerAI->Possess(SpawnedActor);
}
