// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnnemyBase.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "RangedWeapon.generated.h"

UCLASS()
class MYPROJECT_API ARangedWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangedWeapon();	 

	void SetPC(APlayerController* PlayerController) {
        PC = PlayerController;
    };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:	
	virtual void OnFire();
	FRotator SpawnRotation;
	FVector SpawnLocation;
    
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = Controller)
        APlayerController* PC;


    UPROPERTY(EditDefaultsOnly, Category = Weapon)
        FString WeaponName;
    
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		class UPaperFlipbookComponent* GunSprite;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		class UPaperFlipbook* GunMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
		class USceneComponent* MuzzleLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
        FVector GunOffset;
    
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AActorToSpawn> Projectile;


    virtual void RotateGun(float DeltaTime);
	virtual FRotator GetGunRotation() { return GetActorRotation(); }
};
