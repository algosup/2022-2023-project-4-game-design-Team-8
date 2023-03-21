// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnnemyBase.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"

#include "RangedWeapon.generated.h"

class AMyProject2DCharacter;
UCLASS()
class MYPROJECT_API ARangedWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangedWeapon();
private:
    bool bCanShoot = true;
    APlayerController* PC;
    
    float WeaponDamage = 0.f;
    float WeaponFireRate = 0.f;
    
    virtual void RotateGun(float DeltaTime);
    virtual void AllowShoot();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
    FTimerHandle TimerHandler;
public:
    void SetPC(APlayerController* PlayerController) {
        PC = PlayerController;
    };
	virtual void OnFire(FSimpleDelegate IncreasePowerBarDelegate, float PlayerDamage, float PlayerFireRate);

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
		TSubclassOf<class AProjectile> Projectile;

	virtual FRotator GetGunRotation() { return GetActorRotation(); }
};
