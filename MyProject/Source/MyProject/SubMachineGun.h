// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangedWeapon.h"
#include "SubMachineGun.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ASubMachineGun : public ARangedWeapon
{
	GENERATED_BODY()
    
private:
    bool bCanShoot = true;
    float WeaponDamage = 10.f;
    float WeaponFireRate = 4.f;
 public:
     // Sets default values for this actor's properties
     ASubMachineGun();
     void OnFire(FSimpleDelegate IncreasePowerBarDelegate, float PlayerDamage, float PlayerFireRate);

 protected:
     // Called when the game starts or when spawned
     virtual void BeginPlay() override;
     virtual void AllowShoot() override;

};
