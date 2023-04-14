// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

UItem::UItem()
{
	ItemName = "Default";
//	ItemFlipbookComponent->SetFlipbook(ItemFlipbook);
}

void UItem::SetStats(float SpeedUp,float DamageUp,float FireRateUp,float MaxHealthUp,float PowerBarMultiplierUp,float HealthUp)
{
    Speed = SpeedUp;
    Damage = DamageUp;
    FireRate = FireRateUp;
    MaxHealth = MaxHealthUp;
    PowerBarMultiplier = PowerBarMultiplierUp;
    Health = HealthUp;
}


void UItem::OnPickup(AMyProject2DCharacter* Player)
{
    Player->PlayerSpeedUp += Speed;
    Player->PlayerDamageUp += Damage;
    Player->PlayerFireRateUp += FireRate;
    Player->MaxHealthUp += MaxHealth;
    Player->PowerBarMultiplierUp += PowerBarMultiplier;
    Player->HealthUp += Health;
}

void UItem::DisplayName(){
    UE_LOG(LogTemp,Warning,TEXT("My Name Is %s"), *ItemName);
}
