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
    PowerBar->SetPercent(Owner->GetPowerBar()/100.f);
    FNumberFormattingOptions Opts;
    Opts.SetMaximumFractionalDigits(0);
    CurrentHealthLabel->SetText(FText::AsNumber(Owner->GetHealth(), &Opts));
    MaxHealthLabel->SetText(FText::AsNumber(Owner->GetMaxHealth(), &Opts));
    PlayerWeapon = Owner->GetRangedWeapon()->GunFlipbookComponent;
}
