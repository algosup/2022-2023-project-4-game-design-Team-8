// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"


#include "MyProject2DCharacter.h"
#include "EnnemyBase.h"
#include "MyProjectPlayerController.h"
#include "UserInterface.h"
#include "PickableWeapon.h"
#include "Item.h"
#include "SubMachineGun.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h" 
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetArrayLibrary.h"
#include "UObject/UObjectGlobals.h"
#include "Containers/Map.h"

#include "PaperTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileLayer.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"

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


    FLatentActionInfo LatentInfo;
    UGameplayStatics::LoadStreamLevel(GetWorld(), "TopDownExampleMap", true, true, LatentInfo);
    
//    void UItem::SetStats(float SpeedUp,float DamageUp,float FireRateUp,float MaxHealthUp,float PowerBarMultiplierUp,float HealthUp)
    // UItem* DmgUp= NewObject<UItem>( UItem::StaticClass());
    // DmgUp->SetStats(0.f,1.f,0.f,0.f,0.f,0.f);
    // ItemInstances.Emplace("DamageUp",DmgUp);
    // DmgUp->DisplayName();
    // UE_LOG(LogTemp,Warning,TEXT("maybe did displé"));

    // UItem* zeitgj = *ItemInstances.Find("DamageUp");
    // zeitgj->DisplayName();
    // UE_LOG(LogTemp,Warning,TEXT("zeitgj maybe did displé"));

    // UItem* FireRateUp= NewObject<UItem>( UItem::StaticClass());
    // FireRateUp->SetStats(0.f,0.f,0.5f,0.f,0.f,0.f);
    // ItemInstances.Emplace("FireRateUp",FireRateUp);

    // UItem* BigDmgUp= NewObject<UItem>( UItem::StaticClass());
    // BigDmgUp->SetStats(0.f,3.f,0.f,0.f,0.f,0.f);
    // ItemInstances.Emplace("DamageUp2",BigDmgUp);
    // UE_LOG(LogTemp, Warning, TEXT("Len de ItemInstances %d"),ItemInstances.Num());

    // for (auto Itr = ItemInstances.CreateIterator(); Itr; ++Itr)
    // {
    //     UE_LOG(LogTemp,Warning,TEXT("Key %s,"),*Itr.Key());
    //     Itr.Value()->DisplayName();
    // }
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
    // if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandler))
    // {
    //     GetWorldTimerManager().SetTimer(TimerHandler, this, &AMyProjectGameMode::SpawnEnnemies,5.f,true);
    // }
    /*if (APickableWeapon* Weapon = GetWorld()->SpawnActor<APickableWeapon>(PickableWeapon))
    {
        Weapon->SetActorRelativeLocation(FVector(300.f,200.f,33.f));
        UClass* SubMachineClass = ASubMachineGun::StaticClass();
        Weapon->WeaponClass = SubMachineClass;
    }*/
    // UE_LOG(LogTemp, Warning, TEXT("Len de ItemInstances in beginplay %d"),ItemInstances.Num());
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

void AMyProjectGameMode::OpenDoor(FVector PlayerPosition,ARoom* Room,FVector PlayerLastInput,AMyProject2DCharacter* Character)
{
    if(!doorclosed)return;
    int TileX = 0;
    int TileY = 0;
    int Height = 0;
    int Width = 0;
    int NumLayers = 0;
    int DoorsClosed = 1;
    int UpperDoorTileY = 0;
    int UpperDoorTileX = 0;
    
    PlayerPosition.Z = -PlayerPosition.Y;
    
    Room->GetRenderComponent()->GetMapSize(Width,Height,NumLayers);
    FPaperTileInfo Tile;
    FPaperTileInfo* TileInfo = new FPaperTileInfo();
    FPaperTileInfo* UpperDoorTileInfo = new FPaperTileInfo();
    
    Room->GetRenderComponent()->TileMap->GetTileCoordinatesFromLocalSpacePosition(PlayerPosition,TileX,TileY);

    TileY += PlayerLastInput.Y;
    UpperDoorTileY = TileY - 1;
    TileX += PlayerLastInput.X;
    UpperDoorTileX = TileX;
    
    Tile = Room->GetRenderComponent()->GetTile(TileX, TileY, DoorsClosed);
    TileInfo->TileSet = Tile.TileSet;
    TileInfo->PackedTileIndex = Tile.PackedTileIndex;
    
    
    UpperDoorTileInfo->TileSet = Room->GetRenderComponent()->GetTile(UpperDoorTileX,UpperDoorTileY,DoorsClosed).TileSet;
    UpperDoorTileInfo->PackedTileIndex = Room->GetRenderComponent()->GetTile(UpperDoorTileX,UpperDoorTileY,DoorsClosed).PackedTileIndex;
    
    if(TileInfo->PackedTileIndex != -1 && TileInfo->TileSet->GetTileUserData(TileInfo->PackedTileIndex).ToString() == "DoorClosed"){
        TileInfo->PackedTileIndex += 4;
        Room->GetRenderComponent()->SetTile(TileX,TileY,DoorsClosed,*TileInfo);}
    if(UpperDoorTileInfo->PackedTileIndex != -1&& UpperDoorTileInfo->TileSet->GetTileUserData(UpperDoorTileInfo->PackedTileIndex).ToString() == "DoorClosed"){
        UpperDoorTileInfo->PackedTileIndex += 4;
        Room->GetRenderComponent()->SetTile(UpperDoorTileX,UpperDoorTileY,DoorsClosed,*UpperDoorTileInfo);
    }

    if (Room->EnnemyNumber == 0 && UpperDoorTileInfo->PackedTileIndex != -1)
    {
        //if (Room->GetRenderComponent()->GetTile(UpperDoorTileX, UpperDoorTileY, 2))
        FName UData = Room->GetRenderComponent()->GetTile(UpperDoorTileX, UpperDoorTileY, 2).TileSet->GetTileUserData(Room->GetRenderComponent()->GetTile(UpperDoorTileX, UpperDoorTileY, 2).PackedTileIndex);
        ARoom* CurrentRoom = *(Map.Find(CurrentRoomCoord));
        CurrentRoom->GetRenderComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        CurrentRoom->GetRenderComponent()->SetHiddenInGame(true);
        CurrentRoom->GetRenderComponent()->RebuildCollision();

        ARoom* NewRoom;
        FVector NewPos = FVector::ZeroVector;
        if (UData == "doorN")
        {
            CurrentRoomCoord -= 100;
            NewRoom = *(Map.Find(CurrentRoomCoord));
            for (FTileCoordinate door : NewRoom->DoorTiles)
            {
                UE_LOG(LogTemp, Warning, TEXT("log N userdata : %s"), *(door.UserData));
                if (door.UserData == "doorS")
                {
                    UE_LOG(LogTemp, Warning, TEXT("log N IN"));
                    UE_LOG(LogTemp, Warning, TEXT("TileY TileX %d %d"), door.Y, door.X);
                    NewPos = NewRoom->GetRenderComponent()->TileMap->GetTilePositionInLocalSpace(door.X, (-door.Y));
                    break;
                }
            }
        }
        else if (UData == "doorE")
        {
            CurrentRoomCoord += 1;
            NewRoom = *(Map.Find(CurrentRoomCoord));
            for (FTileCoordinate door : NewRoom->DoorTiles)
            {
                UE_LOG(LogTemp, Warning, TEXT("log E"));
                if (door.UserData == "doorW")
                {
                    UE_LOG(LogTemp, Warning, TEXT("log E In"));
                    UE_LOG(LogTemp, Warning, TEXT("TileY TileX %d %d"), door.Y, door.X);
                    NewPos = NewRoom->GetRenderComponent()->TileMap->GetTilePositionInLocalSpace(door.X, (-door.Y));
                    break;
                }
            }
        }
        else if (UData == "doorS")
        {
            CurrentRoomCoord += 100;
            NewRoom = *(Map.Find(CurrentRoomCoord));
            for (FTileCoordinate door : NewRoom->DoorTiles)
            {
                UE_LOG(LogTemp, Warning, TEXT("log S"));
                if (door.UserData == "doorN")
                {
                    UE_LOG(LogTemp, Warning, TEXT("log S In"));
                    UE_LOG(LogTemp, Warning, TEXT("TileY TileX %d %d"), door.Y, door.X);
                    NewPos = NewRoom->GetRenderComponent()->TileMap->GetTilePositionInLocalSpace(door.X, (door.Y - 4));
                    break;
                }
            }
        }
        else if (UData == "doorW")
        {
            CurrentRoomCoord -= 1;
            NewRoom = *(Map.Find(CurrentRoomCoord));
            for (FTileCoordinate door : NewRoom->DoorTiles)
            {
                UE_LOG(LogTemp, Warning, TEXT("log W"));
                if (door.UserData == "doorE")
                {
                    UE_LOG(LogTemp, Warning, TEXT("log W IN"));
                    UE_LOG(LogTemp, Warning, TEXT("TileY TileX %d %d"), door.Y, door.X);
                    NewPos = NewRoom->GetRenderComponent()->TileMap->GetTilePositionInLocalSpace(door.X, (-door.Y));
                    break;
                }
            }
        }
        else
        {
            NewRoom = nullptr;
        }
        if (NewRoom != nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("TileY %d"), CurrentRoomCoord);
            NewPos.Y = NewPos.Z;
            NewPos.Z = Character->GetActorLocation().Z;
            Character->SetActorLocation(NewPos);
            NewRoom->GetRenderComponent()->SetHiddenInGame(false);
            NewRoom->GetRenderComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            NewRoom->GetRenderComponent()->RebuildCollision();
            NewRoom->SpawnEnnemies();
        }
    }
    Room->GetRenderComponent()->RebuildCollision();
}

void AMyProjectGameMode::SpawnEnnemies(FVector EnnemySpawnVector)
{
        FActorSpawnParameters ActorSpawnParams;
        ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        AEnnemyBase* SpawnedActor = GetWorld()->SpawnActor<AEnnemyBase>(Ennemy, EnnemySpawnVector, FRotator(0.f, 0.f, 0.f), ActorSpawnParams);
        AEnnemyAIController* PlayerAI = GetWorld()->SpawnActor<AEnnemyAIController>(MyAIControllerClass);
        PlayerAI->Possess(SpawnedActor);    
}

void AMyProjectGameMode::EnnemyDeath()
{
    (*Map.Find(CurrentRoomCoord))->EnnemyNumber--;
    if ((*Map.Find(CurrentRoomCoord))->EnnemyNumber <= 0)
    {
        (*Map.Find(CurrentRoomCoord))->cleared = true;
    }
}