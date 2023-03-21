// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject2DCharacter.h"
#include "Yul.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AYul : public AMyProject2DCharacter
{
	GENERATED_BODY()
	
private:
    float PlayerSpeed;
    float PlayerDamage;
    float PlayerFireRate;
    float MaxHealth;
    float Health;
    
    bool bIsFiring = false;
    bool bCanTakeDamage = true;
    void BecomeVulnerable();
    float PowerBar;
    FTimerHandle TimerHandler;
public:
    AYul();
public:
    void DecreasePowerBar() override;
    void IncreasePowerBar() override;
};