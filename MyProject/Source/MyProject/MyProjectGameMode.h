// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UserInterface.h"
#include "EnnemyAIController.h"
#include "MyProjectGameMode.generated.h"

UCLASS(minimalapi)
class AMyProjectGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMyProjectGameMode();
    
protected:
    UPROPERTY(EditAnywhere,Category = Interface)
        TSubclassOf<UUserWidget> Interface;
    
    FTimerHandle TimerHandler;
    UPROPERTY(EditAnywhere,Category = Runtime)
    class UUserInterface* UserInterface;
    
    TSubclassOf<class AEnnemyBase> Ennemy;
    
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AEnnemyAIController> MyAIControllerClass;

    
    void SpawnEnnemies();
    virtual void BeginPlay() override;
};
