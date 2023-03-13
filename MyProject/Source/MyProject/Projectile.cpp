// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "EnnemyBase.h"

#include "PhysicsEngine/BodyInstance.h"

#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
AProjectile::AProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //Creating our Default Components
    ProjectileSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sphere Sprite"));
    RootComponent = ProjectileSprite;
    //    SphereComp->bConstrainToPlane = true;
    ProjectileSprite->SetConstraintMode(EDOFMode::XYPlane);
    ProjectileSprite->SetSimulatePhysics(true);

    ProjectileHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Hitbox"));
    ProjectileHitbox->SetRelativeScale3D(FVector(0.1f,0.1f,0.1f));
    ProjectileHitbox->SetupAttachment(RootComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projetcile Movement"));
    ProjectileMovement->UpdatedComponent = ProjectileSprite;
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = false;
    ProjectileMovement->bShouldBounce = false;
    //ProjectileMovement->ProjectileGravityScale = 0;

    ProjectileMovement->Velocity = FVector(1.0f, 1.0f, 0.0f);
    InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
    ProjectileSprite->SetFlipbook(Projectile);
    ProjectileSprite->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnHit);
    ProjectileHitbox->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Warning, TEXT("hit"));

   if(AEnnemyBase* ennemy = Cast<AEnnemyBase>(OtherActor))
   {
       ennemy->Hit(this);
       Destroy();
   }
}

