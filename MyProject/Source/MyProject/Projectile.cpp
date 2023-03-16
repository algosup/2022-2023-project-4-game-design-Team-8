// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "EnnemyBase.h"

#include "PhysicsEngine/BodyInstance.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
AProjectile::AProjectile(const FObjectInitializer& PCIP) : Super(PCIP)
{
    struct FConstructorStatics
    {
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> projectileAsset;
        FConstructorStatics() : projectileAsset(TEXT("PaperFlipbook'/Game/2DSideScroller/Sprites/Bullet.Bullet'")){}
    };
    static FConstructorStatics ConstructorStatics;
    
    Projectile = ConstructorStatics.projectileAsset.Get();

    //Creating our Default Components
    ProjectileSprite = PCIP.CreateDefaultSubobject<UPaperFlipbookComponent>(this,TEXT("Projectile Sprite"));
    RootComponent = ProjectileSprite;
    //    SphereComp->bConstrainToPlane = true;
    ProjectileSprite->SetConstraintMode(EDOFMode::XYPlane);
    ProjectileSprite->SetSimulatePhysics(true);
    
    ProjectileHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sphere Hitbox"));
    
    ProjectileHitbox->SetCapsuleRadius(6.0f);
    ProjectileHitbox->SetCapsuleHalfHeight(6.0f);
    ProjectileHitbox->SetupAttachment(RootComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
    ProjectileMovement->UpdatedComponent = ProjectileSprite;
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = false;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->SetPlaneConstraintEnabled(true);
    ProjectileMovement->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);
//    ProjectileMovement->Velocity = FVector(1.f,1.f,0.f);
    //ProjectileMovement->ProjectileGravityScale = 0;

    InitialLifeSpan = 3.0f;
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
    ProjectileMovement->Velocity = -ProjectileMovement->Velocity;
    ProjectileSprite->SetRelativeRotation(FRotator(0.f,0.f,90.f));
    ProjectileSprite->SetFlipbook(Projectile);
    
    ProjectileHitbox->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);:
    ProjectileHitbox->OnComponentEndOverlap.AddDynamic(this, &AProjectile::OnEndOverlap);
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
   if(AEnnemyBase* ennemy = Cast<AEnnemyBase>(OtherActor))
   {
       ennemy->Hit(this);
       Destroy();
   }
}

void AProjectile::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
   if(AEnnemyBase* ennemy = Cast<AEnnemyBase>(OtherActor))
   {
       ennemy->Hit(this);
       Destroy();
   }
}

