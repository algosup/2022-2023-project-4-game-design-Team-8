// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableItem.h"
#include "RangedWeapon.h"

#include "PickableWeapon.generated.h"

UCLASS()
class MYPROJECT_API APickableWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    APickableWeapon(const FObjectInitializer& PCIP);

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
        TSubclassOf<class ARangedWeapon> WeaponClass;
    class ARangedWeapon* RangedWeapon;
    
    // The animation to play while awaiting pickup
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleFrontAnimation;
    
    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Projectile)
        class UPaperFlipbookComponent* WeaponSprite;
    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Projectile)
        class UPaperFlipbook* WeaponFlipbook;
};
