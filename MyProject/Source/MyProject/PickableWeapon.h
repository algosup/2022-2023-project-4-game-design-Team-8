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
    //virtual void Tick(float DeltaTime) override;
    void PickedUp();
    UFUNCTION(BlueprintCallable)
    void InitWeapon();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
        TSubclassOf<class ARangedWeapon> WeaponClass;
    class ARangedWeapon* RangedWeapon;
   
};
