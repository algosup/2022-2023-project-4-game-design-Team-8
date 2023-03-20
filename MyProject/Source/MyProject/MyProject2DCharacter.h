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
protected:
    float Health;
    float MaxHealth = 200.f;
    
    virtual void BeginPlay() override;
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

    /** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

    

    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	/** Called for side to side input */
	virtual void MoveRight(float Value);
    /** Called for side to side input */
    virtual void MoveUp(float Value);

	virtual void DecrementHealth(int damage);
	virtual void Die();
    
    FSimpleDelegate IncreasePowerBarDelegate;
    void OnFire();
public:
	AMyProject2DCharacter(const FObjectInitializer& PCIP);
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    
    float GetHealth(){ return Health;}
    float GetMaxHealth(){ return MaxHealth;}

    virtual void Hit(AEnnemyBase* ennemy);
    virtual void DecreasePowerBar();
    virtual void IncreasePowerBar();
    void Power();
    FTimerHandle TimerHandler;

    float GetPowerBar() { return PowerBar; }
    ARangedWeapon* GetRangedWeapon() { return RangedWeapon;}
private:
    bool bCanTakeDamage = true;
    void BecomeVulnerable();
    float PowerBar = 0.f;
    
};
