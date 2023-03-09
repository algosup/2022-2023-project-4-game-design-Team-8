// Fill out your copyright notice in the Description page of Project Settings.


#include "EnnemyBase.h"
#include "MyProject2DCharacter.h"
#include "ActorToSpawn.h"

// Sets default values
AEnnemyBase::AEnnemyBase()
{

}


// Called when the game starts or when spawned
void AEnnemyBase::BeginPlay()
{
	Super::BeginPlay();
//    RootComponent->SetRelativeScale3D(FVector(0.4f,0.4f,0.4f));
}

void AEnnemyBase::Hit(AActorToSpawn* projectile)
{
	DecrementHealth(projectile->DamageValue);
}

void AEnnemyBase::HitPlayer(AMyProject2DCharacter* player)
{
    player->Hit(this);
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
