// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject2DCharacter.h"
#include "Item.generated.h"
/**
 * 
 */
UCLASS()
class MYPROJECT_API UItem : public UObject
{
    GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
		FString ItemName;
public:
	UItem();
	virtual void OnPickup(AMyProject2DCharacter* Player);
    void SetStats(float SpeedUp,float DamageUp,float FireRateUp,float MaxHealthUp,float PowerBarMultiplierUp,float HealthUp);


	UPROPERTY(EditAnyWhere,BlueprintReadWrite, Category = Item)
		class UPaperFlipbook* ItemFlipbook;
    
    UPROPERTY(EditDefaultsOnly, Category = Stats)
        float Damage;
    UPROPERTY(EditDefaultsOnly, Category = Stats)
        float FireRate;
    UPROPERTY(EditDefaultsOnly, Category = Stats)
        float Speed;
    UPROPERTY(EditDefaultsOnly, Category = Stats)
        float MaxHealth;
    UPROPERTY(EditDefaultsOnly, Category = Stats)
        float PowerBarMultiplier;
    UPROPERTY(EditDefaultsOnly, Category = Stats)
        float Health;
    
};
