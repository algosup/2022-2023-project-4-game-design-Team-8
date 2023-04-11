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
    virtual void RotateGun(float DeltaTime);
    virtual void AllowShoot();
protected:
    bool bCanShoot = true;
    APlayerController* PC;

    float WeaponDamage = 0.f;
    float WeaponFireRate = 0.f;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
    FTimerHandle TimerHandler;

    virtual float GetWeaponDamage() { return WeaponDamage; }
    virtual float GetWeaponFireRate() { return WeaponFireRate; }
    virtual bool GetbCanShoot() { return bCanShoot; }
    virtual FTimerHandle& GetTimerHandler() { return TimerHandler; }
public:
    void SetPC(APlayerController* PlayerController) {
        PC = PlayerController;
    };
	virtual void OnFire(FSimpleDelegate IncreasePowerBarDelegate);

    UPROPERTY(EditDefaultsOnly, Category = Weapon)
        FString WeaponName;
    
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		class UPaperFlipbookComponent* GunFlipbookComponent;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		class UPaperFlipbook* GunFlipbook;
    UPROPERTY(EditDefaultsOnly, Category = Weapon)
		class UPaperFlipbook* MuzzleFlash;

	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
		class USceneComponent* MuzzleLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
        FVector GunOffset;
    
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> Projectile;

    virtual TSubclassOf<class AProjectile> GetProjectile() { return Projectile; }
	virtual FRotator GetGunRotation() { return GetActorRotation(); }
};
