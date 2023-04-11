// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableWeapon.h"
#include "MyProject2DCharacter.h"

// Sets default values
APickableWeapon::APickableWeapon()
{
    SetActorRelativeRotation(FRotator(0.f, 0.f, -90.f));
}

void APickableWeapon::BeginPlay(){
    Super::BeginPlay();
    SetActorRelativeRotation(FRotator(0.f, 0.f, -90.f));
}

void APickableWeapon::PickedUp()
{
    Destroy();
}

void APickableWeapon::InitWeapon()
{
    UE_LOG(LogTemp, Warning, TEXT("INIT"));
    if (ARangedWeapon* Weapon = GetWorld()->SpawnActor<ARangedWeapon>(WeaponClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("SPawn"));
        RangedWeapon = Weapon;
        RangedWeapon->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        RangedWeapon->SetActorRelativeLocation(FVector(-20.f, 0.f, -8.f));
        RangedWeapon->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));
    }
}
