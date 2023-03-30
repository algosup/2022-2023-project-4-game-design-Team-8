// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableWeapon.h"
#include "MyProject2DCharacter.h"

// Sets default values
APickableWeapon::APickableWeapon(const FObjectInitializer& PCIP) : Super(PCIP)
{
    
//    struct FConstructorStatics
//    {
//        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> weaponAsset;
//        FConstructorStatics() : weaponAsset(TEXT("PaperFlipbook'/Game/2DSideScroller/Sprites/MP5.MP5'")){}
//    };
//    static FConstructorStatics ConstructorStatics;

//    WeaponFlipbook = ConstructorStatics.weaponAsset.Get();
    SetActorRelativeRotation(FRotator(0.f, 0.f, -90.f));
    //Creating our Default Components
//    WeaponSprite = PCIP.CreateDefaultSubobject<UPaperFlipbookComponent>(this,TEXT("Weapon Sprite"));
//    RootComponent = WeaponSprite;
//    RangedWeapon->AttachToComponent(RootComponent,FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void APickableWeapon::BeginPlay(){
    Super::BeginPlay();
    SetActorRelativeRotation(FRotator(0.f, 0.f, -90.f));
}

//void APickableWeapon::Tick(float DeltaTime)
//{
//    Super::Tick(DeltaTime);
//    if (RangedWeapon == nullptr)
//    {
//        InitWeapon();
//    }
//}

void APickableWeapon::PickedUp()
{
    Destroy();
}

void APickableWeapon::InitWeapon()
{
    if (ARangedWeapon* Weapon = GetWorld()->SpawnActor<ARangedWeapon>(WeaponClass))
    {
        RangedWeapon = Weapon;
        RangedWeapon->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        RangedWeapon->SetActorRelativeLocation(FVector(-20.f, 0.f, -8.f));
        RangedWeapon->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));

    }
}
