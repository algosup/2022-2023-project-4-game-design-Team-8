// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnnemyBase.h"
#include "RangedWeapon.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Gun.generated.h"

UCLASS()
class MYPROJECT_API AGun : public ARangedWeapon
{
    GENERATED_BODY()
    
public:
    // Sets default values for this actor's properties
    AGun();


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
//    virtual void Tick(float DeltaSeconds) override;

public:
//    virtual void OnFire() override;
//    void RotateGun(float DeltaTime) ;
//    FRotator GetGunRotation() { return GetActorRotation(); }
};
