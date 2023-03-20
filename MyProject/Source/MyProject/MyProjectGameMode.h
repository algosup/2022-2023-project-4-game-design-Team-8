// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UserInterface.h"
#include "MyProjectGameMode.generated.h"

UCLASS(minimalapi)
class AMyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyProjectGameMode();
    
protected:
    UPROPERTY(EditAnywhere,Category = Interface)
        TSubclassOf<UUserWidget> Interface;
    
    UPROPERTY(EditAnywhere,Category = Runtime)
    class UUserInterface* UserInterface;
    
    virtual void BeginPlay() override;
};
