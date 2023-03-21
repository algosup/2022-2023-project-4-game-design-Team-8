// Fill out your copyright notice in the Description page of Project Settings.


#include "EnnemyBase.h"
#include "MyProject2DCharacter.h"
#include "Projectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperFlipbookComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
// Sets default values
AEnnemyBase::AEnnemyBase()
{
    Health = MaxHealth;
    PrimaryActorTick.bCanEverTick = true;
    //CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
    GetCapsuleComponent()->SetCapsuleHalfHeight(28.0f);
    GetCapsuleComponent()->SetCapsuleRadius(28.0f);
    GetCapsuleComponent()->BodyInstance.bLockXRotation = true;
    GetCapsuleComponent()->BodyInstance.bLockYRotation = true;
    
    /*GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, -1.0f));*/
    GetSprite()->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
//    GetSprite()->SetRelativeLocation(FVector(0.f,-9.f,0.f));
}
// Called when the game starts or when spawned
void AEnnemyBase::BeginPlay()
{
	Super::BeginPlay();

    CapsuleComp = GetCapsuleComponent();
    CapsuleComp->OnComponentHit.AddDynamic(this, &AEnnemyBase::OnHit);
}

// Called every frame
void AEnnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AEnnemyBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (AMyProject2DCharacter* player = Cast<AMyProject2DCharacter>(OtherActor))
    {
        player->Hit(this);
    }
}

void AEnnemyBase::Hit(AProjectile* projectile)
{
	DecrementHealth(projectile->DamageValue);
}


void AEnnemyBase::DecrementHealth(int damage)
{
    Health -= damage;

    if(Health <= 0.f)
    {
        Die();
    }
}
void AEnnemyBase::Die()
{
    Destroy();
}
