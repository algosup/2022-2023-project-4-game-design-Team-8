// Fill out your copyright notice in the Description page of Project Settings.


#include "EnnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnnemyBase.h"

AEnnemyAIController::AEnnemyAIController()
{
    BBC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("UBlackboardComponent"));
    BTC = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("UBehaviorTreeComponent"));
}

void AEnnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    
    AEnnemyBase* Char = Cast<AEnnemyBase>(InPawn);
    if (Char != nullptr && Char->TreeAsset != nullptr)
    {
        BBC->InitializeBlackboard(*Char->TreeAsset->BlackboardAsset);
        
        EnnemyKeyId = BBC->GetKeyID("TargetActor");
        
        BTC->StartTree(*Char->TreeAsset);
    }

}
