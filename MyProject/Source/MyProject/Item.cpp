// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

Item::Item()
{
	ItemName = "Default";
	ItemFlipbookComponent->SetFlipbook(ItemFlipbook);
}

Item::~Item()
{
	ItemName = "Default";
	ItemFlipbookComponent->SetFlipbook(ItemFlipbook);
}

void Item::OnPickup(AMyProject2DCharacter* Player)
{
}
