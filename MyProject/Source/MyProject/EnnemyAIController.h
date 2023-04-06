// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AEnnemyAIController : public AAIController
{
	GENERATED_BODY()
	
    AEnnemyAIController();
public:
    virtual void OnPossess(APawn* InPawn) override;
    
    UPROPERTY(transient)
    class UBehaviorTreeComponent* BTC;
    
    UPROPERTY(transient)
    class UBlackboardComponent* BBC;
    
    uint8 EnnemyKeyId;
};
