// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectile.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PaperCharacter.h"
#include "EnnemyBase.generated.h"

class AMyProject2DCharacter;
UCLASS()
class MYPROJECT_API AEnnemyBase : public APaperCharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AEnnemyBase();

    virtual void Tick(float DeltaTime) override;

    void Hit(AProjectile* projectile);
//    void Hit(MeleeWeapon* projectile);
	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* CapsuleComp;
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunningAnimation;

    // The animation to play while idle (standing still)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleBackAnimation;

    // The animation to play while idle (standing still)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleLeftAnimation;// The animation to play while idle (standing still)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleRightAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleFrontAnimation;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(EditAnywhere)
		float DamageValue = 10.0f;
    
    float GetHealth(){ return Health;}
    float GetMaxHealth(){ return MaxHealth;}
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    void DecrementHealth(int damage);
    void Die();
    float Health;
    float MaxHealth = 100.f;
};
