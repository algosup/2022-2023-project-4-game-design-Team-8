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

	void SetPC(APlayerController* PlayerController) { PC = PlayerController; };

protected:
	ARangedWeapon(const FObjectInitializer& PCIP);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:	
	void OnFire();
	FRotator SpawnRotation;
	FVector SpawnLocation;
	APlayerController* PC;


	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class UPaperFlipbookComponent* GunSprite;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class UPaperFlipbook* GunMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleLocation;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AActorToSpawn> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	void RotateGun(float DeltaTime);
	FRotator GetGunRotation() { return GetActorRotation(); }
};
