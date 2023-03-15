// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class MYPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile(const FObjectInitializer& PCIP);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Projectile)
		class UPaperFlipbookComponent* ProjectileSprite;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Projectile)
		class UPaperFlipbook* Projectile;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = Projectile)
		class UCapsuleComponent* ProjectileHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = Movement)
		class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
    void OnEndHit(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
		float DamageValue = 20.0f;
};
