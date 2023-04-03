// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UserInterface.h"
#include "EnnemyAIController.h"
#include "PickableWeapon.h"

#include "PaperTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileLayer.h"
#include "PaperTileMap.h"
#include "MyProjectGameMode.generated.h"

UCLASS(minimalapi)
class AMyProjectGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMyProjectGameMode();

    void DropWeapon(ARangedWeapon* RangedWeapon,FVector PickedWeaponLocation);
    void OpenDoor(FVector PlayerPosition,APaperTileMapActor* Tile,FVector PlayerLastInput);
    
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
        TArray<UItem*> ItemInstances;
protected:
    UPROPERTY(EditAnywhere,Category = Interface)
        TSubclassOf<UUserWidget> Interface;
    
    FTimerHandle TimerHandler;
    UPROPERTY(EditAnywhere,Category = Runtime)
    class UUserInterface* UserInterface;
    
    TSubclassOf<class AEnnemyBase> Ennemy;
    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "PickableWeapon")
        TSubclassOf<class APickableWeapon> PickableWeapon;
    
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AEnnemyAIController> MyAIControllerClass;
    bool doorclosed = true;
    
    void SpawnEnnemies();
    virtual void BeginPlay() override;
};
