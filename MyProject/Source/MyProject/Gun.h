 // Fill out your copyright notice in the Description page of Project Settings.

 #pragma once

 #include "CoreMinimal.h"
 #include "RangedWeapon.h"
 #include "Gun.generated.h"

 UCLASS()
 class MYPROJECT_API AGun : public ARangedWeapon
 {
     GENERATED_BODY()
 public:
     // Sets default values for this actor's properties
     AGun();

 protected:
     // Called when the game starts or when spawned
     virtual void BeginPlay() override;
 };
