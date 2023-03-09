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
	PrimaryActorTick.bCanEverTick = true;

    //Creating our Default Components
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    SphereComp->InitSphereRadius(16.0f);
    RootComponent = SphereComp;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projetcile Movement"));
    ProjectileMovement->UpdatedComponent = SphereComp;
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->Velocity = FVector(0.0f, 1.0f, 0.0f);
    InitialLifeSpan = 3.0f;

    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));

    //Attaching the Components and setting physics
    SphereComp->SetupAttachment(RootComponent);
    SphereComp->SetSimulatePhysics(true);
    /*StaticMeshComp->AttachToComponent(SphereComp, FAttachmentTransformRules::KeepRelativeTransform);*/
    //ParticleComp->AttachToComponent(StaticMeshComp, FAttachmentTransformRules::KeepRelativeTransform);

    //Setting the Sphere radius to be of a smaller size in line with the Static Mesh.

    //Setting the Static Mesh Scale and Location to fit the radius of the Sphere.
    //StaticMeshComp->SetRelativeLocation(FVector(0.0, 0.0, -12.0f));
    //StaticMeshComp->SetRelativeScale3D(FVector(0.25, 0.25, 0.25));

    //Using Constructor Helpers to set our Static Mesh Comp with a Sphere Shape.
    //static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    //StaticMeshComp->SetStaticMesh(SphereMeshAsset.Object);

    //Using Constructor Helpers to set our Particle Comp with our Fire Particle Comp.
    //static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleCompAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
    //ParticleComp->SetTemplate(ParticleCompAsset.Object);
}

// Called when the game starts or when spawned
void AActorToSpawn::BeginPlay()
{
	Super::BeginPlay();
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AActorToSpawn::OnHit);
}

// Called every frame
void AActorToSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorToSpawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
   if(AEnnemyBase* ennemy = Cast<AEnnemyBase>(OtherActor))
   {
       ennemy->Hit(this);
       Destroy();
   }
}

