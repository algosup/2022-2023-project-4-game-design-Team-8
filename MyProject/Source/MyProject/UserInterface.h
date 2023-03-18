// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyProject2DCharacter.h"
#include "UserInterface.generated.h"


/**
 * 
 */
UCLASS( Abstract)
class MYPROJECT_API UUserInterface : public UUserWidget
{
	GENERATED_BODY()
public:
    void SetOwner(AMyProject2DCharacter* NewOwner) { Owner = NewOwner;}
protected:
    void NativeTick (const FGeometry& MyGeometry, float InDeltaTime) override;
    
    TWeakObjectPtr<AMyProject2DCharacter> Owner;
    
    UPROPERTY(meta = (BindWidget))
        class UProgressBar* HealthBar;
    
    UPROPERTY(meta = (BindWidget))
        class UTextBlock* CurrentHealthLabel;
    
    UPROPERTY(meta = (BindWidget))
        class UTextBlock* MaxHealthLabel;
    
    UPROPERTY(meta = (BindWidget))
        class UImage* WeaponImage;
    
};
