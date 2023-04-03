// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableItem.h"
#include "Item.h"
#include "PaperFlipbook.h"
// Sets default values
APickableItem::APickableItem()
{
	SetActorRelativeRotation(FRotator(0.f, 0.f, -90.f));
    ItemFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Item Sprite"));
}

void APickableItem::PickedUp()
{
	Destroy();
}

void APickableItem::InitItem(UItem* Item)
{
    /*if (Item* SpawnedItem = GetWorld()->SpawnActor<Item>(ItemClass))
    {
        ItemContained = SpawnedItem;
        ItemContained->ItemFlipbookComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }*/
//    ItemContained = NewObject<UItem>(this,ItemClass);
    ItemContained = Item;
    ItemFlipbookComponent->SetFlipbook(ItemContained->ItemFlipbook);
}
