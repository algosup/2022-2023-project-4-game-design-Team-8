// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "EnnemyBase.h"
#include "MyProjectGameMode.h"

void ARoom::BeginPlay()
{
    Super::BeginPlay();
    cleared = false;
    EnnemyNumber = 0;
}
void ARoom::FindMetaTiles()
{
    FTileCoordinate MapSize;
    MapSize.X = 0;
    MapSize.Y = 0;
    MapSize.Layer = 0;
    GetRenderComponent()->GetMapSize(MapSize.X,MapSize.Y,MapSize.Layer);

    int X = 0;
    int Y = 0;
    int DoorPlacingLayer = 2;
    int EnnemyPlacingLayer = 3;
    while(Y < MapSize.Y)
    {
        FPaperTileInfo DoorTileInfo = GetRenderComponent()->GetTile(X,Y,DoorPlacingLayer);
        FPaperTileInfo EnnemyTileInfo = GetRenderComponent()->GetTile(X,Y,EnnemyPlacingLayer);
        if (DoorTileInfo.PackedTileIndex != -1)
        {
            FTileCoordinate DoorCoordinate;
            DoorCoordinate.X = X;
            DoorCoordinate.Y = Y;
            DoorCoordinate.Layer = DoorPlacingLayer;
            if(DoorTileInfo.TileSet->GetTileMetadata(DoorTileInfo.PackedTileIndex)->HasMetaData())
            {
                DoorCoordinate.UserData = DoorTileInfo.TileSet->GetTileMetadata(DoorTileInfo.PackedTileIndex)->UserDataName.ToString();
            }
            DoorTiles.Add(DoorCoordinate);
        }
        if(EnnemyTileInfo.PackedTileIndex != -1)
        {
            FTileCoordinate EnnemyCoordinate;
            EnnemyCoordinate.X = X;
            EnnemyCoordinate.Y = Y;
            EnnemyCoordinate.Layer = EnnemyPlacingLayer;
            EnnemySpawnTiles.Add(EnnemyCoordinate);
        }
        X++;
        if (X == MapSize.X)
        {
            X = 0;
            Y++;
        }
    }
}

void ARoom::SpawnEnnemies()
{   
    if (!cleared)
    {
        for (auto& Ennemy : EnnemySpawnTiles)
        {
            AMyProjectGameMode* GameMode = (AMyProjectGameMode*)GetWorld()->GetAuthGameMode();
            FVector TileVector = GetRenderComponent()->TileMap->GetTilePositionInLocalSpace(Ennemy.X, Ennemy.Y, Ennemy.Layer);
            TileVector.Y = -TileVector.Z;
            TileVector.Z = 70.f;
            GameMode->SpawnEnnemies(TileVector);
            EnnemyNumber++;
            //UE_LOG(LogTemp, Warning, TEXT("Spawn %d"), EnnemySpawnTiles.Num());
            // AEnnemyAIController* PlayerAI = GetWorld()->SpawnActor<AEnnemyAIController>(MyAIControllerClass);
            // PlayerAI->Possess(SpawnedActor);
        }
    }
}