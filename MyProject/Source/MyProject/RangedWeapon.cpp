// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeapon.h"
#include "Components/BoxComponent.h"
#include "EnnemyBase.h"
#include "TimerManager.h"
// Sets default values
ARangedWeapon::ARangedWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolume->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}


// Called when the game starts or when spawned
void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();
	// GetWorld()->GetTimerManager().SetTimer(SpawnHandle,this,&ARangedWeapon::Spawn,2.0f,true);
}

// Called every frame
void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// GetWorld()->SpawnActor<AEnnemyBase>(Projectile, FVector(0.f,0.f,210.f), FRotator(0.f,0.f,0.f));
}
