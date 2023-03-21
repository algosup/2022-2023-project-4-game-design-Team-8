// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnnemyBase.h"
#include "PaperCharacter.h"
#include "RangedWeapon.h"
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
private:
    bool bCanTakeDamage = true;
    void BecomeVulnerable();
    float PowerBar = 0.f;
    FTimerHandle TimerHandler;

    float PlayerSpeed = 10.f;
    float PlayerDamage = 5.f;
    float PlayerFireRate = 1.f;
    float MaxHealth = 10.f;
    float Health = MaxHealth;
    bool bIsFiring = false;
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

    virtual void BeginPlay() override;
    /** Called to choose the correct animation to play based on the character's movement state */
	virtual void UpdateAnimation();

    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void MoveRight(float Value);
    virtual void MoveUp(float Value);

	virtual void DecrementHealth(int damage);
	virtual void Die();
    
    virtual void StartFire();
    virtual void StopFire();
    virtual void Fire();
public:
	AMyProject2DCharacter();
    
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    ARangedWeapon* GetRangedWeapon() { return RangedWeapon; }
    float GetHealth(){ return Health;}
    float GetMaxHealth(){ return MaxHealth;}
    float GetPowerBar() { return PowerBar; }
        
    virtual void Power();
    
    virtual void Hit(AEnnemyBase* ennemy);
    virtual void DecreasePowerBar();
    virtual void IncreasePowerBar();
};
