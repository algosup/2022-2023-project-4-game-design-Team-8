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
     GunFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("SubMachineGun Sprite"));
     MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SubMachineGun Muzzle Location"));
     MuzzleLocation->SetupAttachment(GunFlipbookComponent);
     WeaponDamage = 10.f;
     WeaponFireRate = 3.f;
}


// Called when the game starts or when spawned
void ASubMachineGun::BeginPlay()
{
     Super::BeginPlay();
     GunFlipbookComponent->SetFlipbook(GunFlipbook);
     MuzzleLocation->SetRelativeLocation(FVector(-0.f, 0.f, 0.f));
}
