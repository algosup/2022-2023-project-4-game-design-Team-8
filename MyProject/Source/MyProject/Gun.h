 // Fill out your copyright notice in the Description page of Project Settings.

 #pragma once

 #include "CoreMinimal.h"
 #include "RangedWeapon.h"
 #include "Gun.generated.h"

 UCLASS()
 class MYPROJECT_API AGun : public ARangedWeapon
 {
     GENERATED_BODY()

 private:
     bool bCanShoot = true;
     float WeaponDamage = 30.f;
     float WeaponFireRate = 1.f;
 public:
     // Sets default values for this actor's properties
     AGun();
     void OnFire(FSimpleDelegate IncreasePowerBarDelegate, float PlayerDamage, float PlayerFireRate);

 protected:
     // Called when the game starts or when spawned
     virtual void BeginPlay() override;
     virtual void AllowShoot() override;

 };
