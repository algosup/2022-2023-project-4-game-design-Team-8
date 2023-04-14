// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActorToSpawn.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnnemyBase.generated.h"

class AMyProject2DCharacter;
UCLASS()
class MYPROJECT_API AEnnemyBase : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AEnnemyBase();

    virtual void Tick(float DeltaTime) override;

    void Hit(AActorToSpawn* projectile);
//    void Hit(MeleeWeapon* projectile);
	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* CapsuleComp;
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* RunningBackAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* RunningLeftAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* RunningRightAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* RunningFrontAnimation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleFrontAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleBackAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleLeftAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleRightAnimation;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(EditAnywhere)
		float DamageValue = 10.0f;
    float Health;
    float MaxHealth = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
        class UBehaviorTree* TreeAsset;
    
    float GetHealth(){ return Health;}
    float GetMaxHealth(){ return MaxHealth;}
    FTimerHandle TimerHandler;

    void ResetColor();
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    void DecrementHealth(int damage);
    void Die();
};
