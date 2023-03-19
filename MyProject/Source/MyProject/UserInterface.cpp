// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "PaperSpriteBlueprintLibrary.h"
#include "Styling/SlateBrush.h"


void UUserInterface::NativeTick (const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    if(!Owner.IsValid()) return;
    
    HealthBar->SetPercent(Owner->GetHealth() / Owner->GetMaxHealth());
    
    FNumberFormattingOptions Opts;
    Opts.SetMaximumFractionalDigits(0);
    CurrentHealthLabel->SetText(FText::AsNumber(Owner->GetHealth(), &Opts));
    MaxHealthLabel->SetText(FText::AsNumber(Owner->GetMaxHealth(), &Opts));
    UPaperSprite* spri = Owner->GetRangedWeapon()->GunMesh->GetSpriteAtFrame(0);
//    WeaponImage->SetBrush(UPaperSpriteBlueprintLibrary::MakeBrushFromSprite(spri,200,200));
    
}
