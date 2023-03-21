// Fill out your copyright notice in the Description page of Project Settings.


#include "Yul.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"

AYul::AYul()
{
    Health = MaxHealth;
    PlayerSpeed = 10.f;
    PlayerDamage = 5.f;
    PlayerFireRate = 1.f;
    MaxHealth = 10.f;
    Health = MaxHealth;
    PowerBar = 0.f;
    IncreasePowerBarDelegate.BindUObject(this,&AYul::IncreasePowerBar);
}

void AYul::DecreasePowerBar()
{
    PowerBar = PowerBar >= 10.f ? PowerBar - 10.f : 0.f;
}
void AYul::IncreasePowerBar()
{
    PowerBar = PowerBar < 100.f ? PowerBar += 1.f : 100.f;
}
