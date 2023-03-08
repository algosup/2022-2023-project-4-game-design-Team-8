// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "MyProject2DCharacter.generated.h"

class UTextRenderComponent;

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
    
	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;
protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);
    
    /** Called for side to side input */
    void MoveUp(float Value);

	void UpdateCharacter();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
    virtual void BeginPlay() override;

    void OnFire();
public:
	AMyProject2DCharacter();
    
    
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
        class USkeletalMeshComponent* GunMesh;
    
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
        class USceneComponent* MuzzleLocation;
    
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class AActorToSpawn> Projectile;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        FVector GunOffset;


	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    FRotator SpawnRotation;
    FVector SpawnLocation;
};
