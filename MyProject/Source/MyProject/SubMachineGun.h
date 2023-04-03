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
 public:
     // Sets default values for this actor's properties
     ASubMachineGun();

 protected:
     // Called when the game starts or when spawned
     virtual void BeginPlay() override;
};
