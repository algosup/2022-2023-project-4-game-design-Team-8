// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "EnnemyBase.h"
#include "MyProject2DCharacter.h"
#include "ActorToSpawn.h"

// Sets default values
AEnnemyBase::AEnnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;
    //CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));

    //CapsuleComp->SetCapsuleHalfHeight(40.0f);
    //CapsuleComp->SetCapsuleRadius(50.0f);
}
// Called when the game starts or when spawned
void AEnnemyBase::BeginPlay()
{
	Super::BeginPlay();

    CapsuleComp = GetCapsuleComponent();
    //CapsuleComp->SetSimulatePhysics(true);
//    RootComponent->SetRelativeScale3D(FVector(0.4f,0.4f,0.4f));
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

void AEnnemyBase::Hit(AActorToSpawn* projectile)
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
