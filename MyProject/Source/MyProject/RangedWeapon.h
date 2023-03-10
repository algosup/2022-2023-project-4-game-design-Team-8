// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnnemyBase.h"
#include "RangedWeapon.generated.h"

UCLASS()
class MYPROJECT_API ARangedWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangedWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AEnnemyBase* actor;
    	// void Spawn();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* SpawnVolume;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AEnnemyBase> Projectile;
    
    FTimerHandle SpawnHandle;
};
