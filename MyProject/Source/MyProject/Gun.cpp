 //
 //  Gun.cpp
 //  MyProject
 //
 //  Created by Chaput Mathieu on 16/03/2023.
 //  Copyright © 2023 Epic Games, Inc. All rights reserved.
 //

#include "Gun.h"
#include "RangedWeapon.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
 AGun::AGun()
 {
//      Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
      PrimaryActorTick.bCanEverTick = true;
      WeaponName = "Gun";
      WeaponDamage = 30.f;
      WeaponFireRate = 1.f;

      GunFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("GUNGun Sprite"));
      MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Gun Muzzle Location"));
      MuzzleLocation->SetupAttachment(GunFlipbookComponent);
 }


 // Called when the game starts or when spawned
 void AGun::BeginPlay()
 {
      Super::BeginPlay();
      GunFlipbookComponent->SetFlipbook(GunFlipbook);
 }
