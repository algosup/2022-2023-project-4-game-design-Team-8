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


    void Hit(AActorToSpawn* projectile);
//    void Hit(MeleeWeapon* projectile);
    void HitPlayer(AMyProject2DCharacter* player);

	UPROPERTY(EditAnywhere)
		float DamageValue = 10.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    void DecrementHealth(int damage);
    void Die();
    float Health = 100.f;
};
