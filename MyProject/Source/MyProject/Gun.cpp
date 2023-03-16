//
//  Gun.cpp
//  MyProject
//
//  Created by Chaput Mathieu on 16/03/2023.
//  Copyright © 2023 Epic Games, Inc. All rights reserved.
//

#include "Gun.h"
#include "RangedWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "EnnemyBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Math/Rotator.h"
#include "TimerManager.h"
AGun::AGun()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    WeaponName = "Gun";
    GunSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("GUNGun Sprite"));
    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Gun Muzzle Location"));
    MuzzleLocation->SetupAttachment(GunSprite);
}


// Called when the game starts or when spawned
void AGun::BeginPlay()
{
    Super::BeginPlay();
    GunSprite->SetFlipbook(GunMesh);
    MuzzleLocation->SetRelativeLocation(FVector(-0.f, 0.f, 0.f));
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Declaration of variables to hold mouse vectors.
    // Pass by reference to get mouse position in world space and direction vector.
    if (PC)
    {
        RotateGun(DeltaTime);
    }
}


//void AGun::OnFire()
//{
//}
