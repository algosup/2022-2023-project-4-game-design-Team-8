// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"

#include "EnnemyBase.h"
#include "MyProject2DCharacter.h"
#include "Yul.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperSpriteComponent.h"

#include "PaperFlipbookComponent.h"
#include "Math/Rotator.h"
#include "TimerManager.h"
ARangedWeapon::ARangedWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    WeaponName = "Default";
}


// Called when the game starts or when spawned
void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (PC)
    {
        RotateGun(DeltaTime);
    }
}


void ARangedWeapon::OnFire(FSimpleDelegate IncreasePowerBarDelegate)
{
     if(GetWorld() != NULL && GetbCanShoot())
     {
         FRotator SpawnRotation = GetActorRotation();
        
         FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) +SpawnRotation.RotateVector(GunOffset);
        
         FActorSpawnParameters ActorSpawnParams;
         ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
         AProjectile* proj = GetWorld()->SpawnActor<AProjectile>(GetProjectile(), SpawnLocation, SpawnRotation, ActorSpawnParams);
         proj->IncreasePowerBarDelegate = IncreasePowerBarDelegate;
         if (AMyProject2DCharacter* owner = Cast<AMyProject2DCharacter>(GetOwner()))
         {
             proj->DamageValue = GetWeaponDamage() + owner->GetPlayerDamage();
             float WaitTimer = 1.f / (GetWeaponFireRate() * owner->GetPlayerFireRate());
             bCanShoot = false;
             if (!GetWorld()->GetTimerManager().IsTimerActive(GetTimerHandler()))
             {
                 GetWorldTimerManager().SetTimer(GetTimerHandler(), this, &ARangedWeapon::AllowShoot, WaitTimer, false);
             }
         }
     }
}

void ARangedWeapon::AllowShoot()
{
    bCanShoot = true;
}

void ARangedWeapon::RotateGun(float DeltaTime)
{
     FVector playerLoc = GetActorLocation();
     FHitResult hitResult;
     PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, hitResult);
     FVector hitLoc = hitResult.Location;
     float newYaw = (hitLoc - playerLoc).Rotation().Yaw;
     FRotator newRot = GetActorRotation();
     newRot.Yaw = newYaw + 180;
     SetActorRotation(newRot);
}
