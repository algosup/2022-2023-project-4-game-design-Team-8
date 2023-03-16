// Fill out your copyright notice in the Description page of Project Settings.


#include "SubMachineGun.h"
#include "RangedWeapon.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "PaperFlipbookComponent.h"
ASubMachineGun::ASubMachineGun()
{
//      Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
     PrimaryActorTick.bCanEverTick = true;
     WeaponName = "SubMachineGun";
     GunSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("SubMachineGun Sprite"));
     MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SubMachineGun Muzzle Location"));
     MuzzleLocation->SetupAttachment(GunSprite);
}


// Called when the game starts or when spawned
void ASubMachineGun::BeginPlay()
{
     Super::BeginPlay();
     GunSprite->SetFlipbook(GunMesh);
     MuzzleLocation->SetRelativeLocation(FVector(-0.f, 0.f, 0.f));
}

// Called every frame
// void ASubMachineGun::Tick(float DeltaTime)
// {
//      Super::Tick(DeltaTime);
//      // Declaration of variables to hold mouse vectors.
//      // Pass by reference to get mouse position in world space and direction vector.
//      if (PC)
//      {
//          RotateGun(DeltaTime);
//      }
// }


//void ASubMachineGun::OnFire()
//{
//}
