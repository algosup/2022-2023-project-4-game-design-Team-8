// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "EnnemyBase.h"
#include "GameFramework/PlayerController.h"
#include "PaperSpriteComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Math/Rotator.h"
#include "TimerManager.h"
ARangedWeapon::ARangedWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    GunSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Gun Sprite"));
    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
    MuzzleLocation->SetupAttachment(GunSprite);
}


// Called when the game starts or when spawned
void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();
    GunSprite->SetFlipbook(GunMesh);
    MuzzleLocation->SetRelativeLocation(FVector(0.0f, 0.f, 0.f));
}

// Called every frame
void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


    // Declaration of variables to hold mouse vectors.
    // Pass by reference to get mouse position in world space and direction vector
  if (PC)
  {
    
    RotateGun(DeltaTime);
  }
    
}

void ARangedWeapon::OnFire()
{
    if(GetWorld() != NULL)
    {
        SpawnRotation = GetActorRotation();
        
        SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) +SpawnRotation.RotateVector(GunOffset);
        
        FActorSpawnParameters ActorSpawnParams;
        ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
        AActorToSpawn* projec = Cast<AActorToSpawn>(Projectile);
        GetWorld()->SpawnActor<AActorToSpawn>(Projectile, SpawnLocation, SpawnRotation, ActorSpawnParams);
    }
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
