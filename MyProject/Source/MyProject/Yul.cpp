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
    IncreasePowerBarDelegate.BindUObject(this,&AYul::IncreasePowerBar);
    UE_LOG(LogTemp, Warning, TEXT("Health : %.2f ; %.2f"), MaxHealth, Health);
}

void AYul::DecreasePowerBar()
{
    PowerBar = PowerBar >= 10.f ? PowerBar - 10.f : 0.f;
}
void AYul::IncreasePowerBar()
{
    PowerBar = PowerBar < 100.f ? PowerBar += 1.f : 100.f;
    UE_LOG(LogTemp, Warning, TEXT("powerbar %.2f"), PowerBar);
}

void AYul::Hit(AEnnemyBase* ennemy)
{
    if (bCanTakeDamage)
    {
        UGameInstance* GI = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        if (GI)
        {
            DecrementHealth(ennemy->DamageValue);
            DecreasePowerBar();
            bCanTakeDamage = false;
            if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandler))
            {
                GetWorldTimerManager().SetTimer(TimerHandler, this, &AYul::BecomeVulnerable, 3.f, false);
            }
        }
    }
}


void AYul::BecomeVulnerable()
{
    bCanTakeDamage = true;
}

void AYul::DecrementHealth(int damage)
{
    Health -= damage;
    if (Health <= 0.f)
    {
        Die();
    }
}

void AYul::Die()
{
    AMyProjectGameMode* GameMode = (AMyProjectGameMode*)GetWorld()->GetAuthGameMode();
    GameMode->RestartGame();
}
