// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeapon.h"
#include "Components/BoxComponent.h"
#include "EnnemyBase.h"
#include "GameFramework/PlayerController.h"
#include "PaperSpriteComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
ARangedWeapon::ARangedWeapon(const class FObjectInitializer& PCIP):Super(PCIP)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    //GunSprite = PCIP.CreateDefaultSubobject<UPaperFlipbookComponent>(this,TEXT("Gun Sprite"));

    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
    MuzzleLocation->SetupAttachment(RootComponent);
    MuzzleLocation->SetRelativeLocation(FVector(0.0f, 56.5f, 11.3f));
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

    // Declaration of variables to hold mouse vectors.
    // Pass by reference to get mouse position in world space and direction vector.
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

    FVector MouseDir = FVector::ZeroVector;
    FVector MousePos = FVector::ZeroVector;
    PC->DeprojectMousePositionToWorld(MousePos, MouseDir);

    // Declaration of vector of intersection.
    FVector Intersection = FVector::ZeroVector;
    float t = 0.f;
    // Vector from camera that crosses the plane we want the intersection.
    FVector LineEnd = MousePos + MouseDir * 2000.f;
    // Get intersection vector. Returns true if intersection was possible.
    bool bIntersectionSuccess = UKismetMathLibrary::LinePlaneIntersection_OriginNormal(
        MousePos,
        LineEnd,
        GetActorLocation(),
        GetActorUpVector(),
        t,
        Intersection
    );
    // Do stuff if line intersected.
    if (bIntersectionSuccess)
    {
        // Calculate direction vector from the pawn body forward vector to intersection vector.
        FVector DirToIntersection = (Intersection - GetActorLocation()).GetSafeNormal();
        // Gets the cosine of the angle between the pawns body forward vector and the direction to intersection.
        float dotForward = GetActorRightVector() | DirToIntersection;
        // Converts the cosine of the angle to degrees.
        float Angle = acos(dotForward) * (180.f / PI);
        // Only update the rotation if it is greater to avoid unwanted behaviour.
        if (Angle > .2f)
        {
            // Clamp to limit how fast the component can rotate.
            //Angle = FMath::Clamp(Angle, 0.f, 25.f);

            // Gets the cosine of the angle with the right vector against direction to intersection to know on what side of the component is the intersection.
            float dotSide = GetActorForwardVector() | DirToIntersection;

            // Negates the value depending on what side is the intersection relative to the component.
            Angle *= 10.f * ((dotSide < 0.f) ? 1.f : -1.f);

            // Create rotator with variable.
            float BodyRotator = Angle * DeltaTime;

            //Set the rotation so that the character faces his direction of travel.
            AddActorLocalRotation(FRotator(BodyRotator,0.f, 0.f));
        }
    }
}