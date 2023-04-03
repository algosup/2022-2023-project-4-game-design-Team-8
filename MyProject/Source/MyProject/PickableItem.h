// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.h"
#include "PickableItem.generated.h"

UCLASS()
class MYPROJECT_API APickableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    APickableItem();
	void PickedUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		TSubclassOf<class UItem> ItemClass;
	class UItem* ItemContained;

    UPROPERTY(EditDefaultsOnly, Category = Item)
        class UPaperFlipbookComponent* ItemFlipbookComponent;
	UFUNCTION(BlueprintCallable)
		void InitItem(UItem* Item);
};
