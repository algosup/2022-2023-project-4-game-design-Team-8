// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapActor.h"
#include "RangedWeapon.h"
#include "PickableWeapon.h"
#include "PaperTileSet.h"
#include "Room.generated.h"

USTRUCT(BlueprintType)
struct FTileCoordinate
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coords")
    int X;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coords")
    int Y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coords")
    int Layer;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coords")
    FString UserData;
    // UFUNCTION(BlueprintCallable)
    void Set(FTileCoordinate Coords){ this->X = Coords.X; this->Y = Coords.Y; this->Layer = Coords.Layer; this->UserData = Coords.UserData; }
};
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coords")
    TArray<FTileCoordinate> DoorTiles;
    UFUNCTION(BlueprintCallable)
        void SpawnEnnemies();

    UFUNCTION(BlueprintCallable)
    void SpawnPickableWeapon(ARangedWeapon* RangedWeapon, FVector PickedWeaponLocation, UClass* PickableWeaponClass, APickableWeapon* Pickable);
    UFUNCTION(BlueprintCallable)
    void SpawnPickable(FVector PickedWeaponLocation, UClass* PickableWeaponClass, UClass* SubMachineClass);
    UFUNCTION(BlueprintCallable)
    void HidePickables();
    void ShowPickables();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coords")
        UPaperTileSet* DoorTileSet;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coords")
        UPaperTileSet*   WallTileSet;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coords")
    TArray<FTileCoordinate> EnnemySpawnTiles;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coords")
    int EnnemyNumber;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coords")
    bool cleared;

    TArray<APickableWeapon*> PickableWeapons;

    virtual void BeginPlay() override;
    UFUNCTION(BlueprintCallable)
    void FindMetaTiles();
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coords")
//    struct FCoords Coordinates;
//    UFUNCTION(BlueprintCallable)
//    void PlaceDoors();
};
