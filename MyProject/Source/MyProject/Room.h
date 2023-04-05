// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapActor.h"
#include "Room.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ARoom : public APaperTileMapActor
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coords")
    int Coordinates;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coords")
    bool bCanHaveNeighbours;
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coords")
//    struct FCoords Coordinates;
//    UFUNCTION(BlueprintCallable)
//    void PlaceDoors();
};
