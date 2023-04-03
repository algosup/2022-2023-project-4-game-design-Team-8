// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnnemyBase.h"
#include "PaperCharacter.h"
#include "RangedWeapon.h"
#include "Item.h"
#include "MyProject2DCharacter.generated.h"

/**
 * This class is the default character for MyProject, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class AMyProject2DCharacter : public APaperCharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
    
    /** A decal that projects to the cursor location. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UDecalComponent* CursorToWorld;
    
	virtual void Tick(float DeltaSeconds) override;
protected:
    bool bCanTakeDamage = true;

    float PowerBar = 0.f;
    FTimerHandle TimerHandler;

    float PlayerSpeed = 10.f;
    float PlayerDamage = 0.f;
    float PlayerFireRate = 1.f;
    float MaxHealth = 10.f;
    float PowerBarMultiplier = 1.f;
    float Health = MaxHealth;
    bool bIsFiring = false;

    //TArray<Item> ItemList;
protected:
    FSimpleDelegate IncreasePowerBarDelegate;
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
        class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleFrontAnimation;
    
    // The animation to play while idle (standing still)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleBackAnimation;
    
    // The animation to play while idle (standing still)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleLeftAnimation;// The animation to play while idle (standing still)
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleRightAnimation;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TSubclassOf<class ARangedWeapon> StartingWeaponClass;
	class ARangedWeapon* RangedWeapon;

    virtual void InitWeapon(ARangedWeapon* Weapon);

    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
   
    /** Called to choose the correct animation to play based on the character's movement state */
	virtual void UpdateAnimation();
    
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void MoveRight(float Value);
    virtual void MoveUp(float Value);
    
    virtual void StartFire();
    virtual void StopFire();
    virtual void Fire();
    
    virtual void BecomeVulnerable();
	virtual void DecrementHealth(int damage);
	virtual void Die();
    
    
public:
	AMyProject2DCharacter();
    
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    ARangedWeapon* GetRangedWeapon() { return RangedWeapon; }
        
    virtual void Power();

    virtual void UnPower();
    
    virtual void Hit(AEnnemyBase* ennemy);
    virtual void DecreasePowerBar();
    virtual void IncreasePowerBar();


    float GetPlayerSpeed() { return PlayerSpeed + PlayerSpeedUp; }
    float GetMaxHealth() { return MaxHealth + MaxHealthUp; }
    float GetHealth() { return Health + HealthUp; }
    float GetPlayerDamage() { return PlayerDamage + PlayerDamageUp; }
    float GetPlayerFireRate() { return PlayerFireRate + PlayerFireRateUp; }
    float GetPowerBar() { return PowerBar; }
    float GetPowerBarMultiplier() { return PowerBarMultiplier + PowerBarMultiplierUp; }

    bool GetbIsFiring() { return bIsFiring; }
    bool GetbCanTakeDamage() { return bCanTakeDamage; }
    
    float PlayerSpeedUp = 0.f;
    float PlayerDamageUp = 0.f;
    float PlayerFireRateUp = 0.f;
    float MaxHealthUp = 0.f;
    float PowerBarMultiplierUp = 0.f;
    float HealthUp = 0.f;
};
