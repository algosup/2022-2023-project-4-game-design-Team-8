// Fill out your copyright notice in the Description page of Project Settings.


#include "Yul.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "MyProjectGameMode.h"
#include "Kismet/GameplayStatics.h"


AYul::AYul()
{
    MaxHealth = 50.f;
    PlayerSpeed = 10.f;
    PlayerDamage = 5.f;
    PlayerFireRate = 1.f;
    Health = MaxHealth;
    PowerBar = 0.f;
    PowerBarMultiplier = 1.f;
}