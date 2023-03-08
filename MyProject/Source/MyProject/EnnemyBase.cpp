// Fill out your copyright notice in the Description page of Project Settings.


#include "EnnemyBase.h"

// Sets default values
AEnnemyBase::AEnnemyBase()
{

}

// Called when the game starts or when spawned
void AEnnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnnemyBase::Hit(AEnnemyBase* ennemy, AActorSpawn projectile)
{
//    ennemy.DecrementHealth(projectile.DamageValue)
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
