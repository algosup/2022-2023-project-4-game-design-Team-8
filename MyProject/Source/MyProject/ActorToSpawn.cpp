// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorToSpawn.h"
#include "EnnemyBase.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
AActorToSpawn::AActorToSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//    //Creating our Default Components
//    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
//    SphereComp->InitSphereRadius(16.0f);
//    RootComponent = SphereComp;
//
//    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projetcile Movement"));
//    ProjectileMovement->UpdatedComponent = SphereComp;
//    ProjectileMovement->InitialSpeed = 3000.0f;
//    ProjectileMovement->MaxSpeed = 3000.0f;
//    ProjectileMovement->bRotationFollowsVelocity = true;
//    ProjectileMovement->bShouldBounce = false;
//    ProjectileMovement->Velocity = FVector(0.0f, 1.0f, 0.0f);
//    InitialLifeSpan = 3.0f;
//
//
//    //Attaching the Components and setting physics
//    SphereComp->SetupAttachment(RootComponent);
//    SphereComp->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AActorToSpawn::BeginPlay()
{
//	Super::BeginPlay();
//    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AActorToSpawn::OnHit);
}

void AActorToSpawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{

//   if(AEnnemyBase* ennemy = Cast<AEnnemyBase>(OtherActor))
//   {
//       ennemy->Hit(this);
//       Destroy();
//   }
}

