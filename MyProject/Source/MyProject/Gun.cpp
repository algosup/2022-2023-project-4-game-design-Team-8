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
      GunSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("GUNGun Sprite"));
      MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Gun Muzzle Location"));
      MuzzleLocation->SetupAttachment(GunSprite);
 }


 // Called when the game starts or when spawned
 void AGun::BeginPlay()
 {
      Super::BeginPlay();
      GunSprite->SetFlipbook(GunMesh);
 }

 void AGun::OnFire(FSimpleDelegate IncreasePowerBarDelegate, float PlayerDamage, float PlayerFireRate)
 {
     if (GetWorld() != NULL && bCanShoot)
     {
         FRotator SpawnRotation = GetActorRotation();

         FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

         FActorSpawnParameters ActorSpawnParams;
         ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
         AProjectile* proj = GetWorld()->SpawnActor<AProjectile>(Projectile, SpawnLocation, SpawnRotation, ActorSpawnParams);
         proj->IncreasePowerBarDelegate = IncreasePowerBarDelegate;
         proj->DamageValue = WeaponDamage + PlayerDamage;
         bCanShoot = false;
         float WaitTimer = 1.f / (WeaponFireRate + PlayerFireRate);
         if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandler))
         {
             GetWorldTimerManager().SetTimer(TimerHandler, this, &AGun::AllowShoot, WaitTimer, false);
         }
     }
 }


 void AGun::AllowShoot()
 {
     bCanShoot = true;
 }
