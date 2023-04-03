// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject2DCharacter.h"
//#include "Item.generated.h"
/**
 * 
 */
//UCLASS()
class MYPROJECT_API Item
{
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
		FString ItemName;
public:
	Item();
	~Item();
	virtual void OnPickup(AMyProject2DCharacter* Player);

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		class UPaperFlipbookComponent* ItemFlipbookComponent;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		class UPaperFlipbook* ItemFlipbook;
};
