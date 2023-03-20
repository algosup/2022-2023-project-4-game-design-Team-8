// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject2DCharacter.h"
#include "PaperFlipbookComponent.h"
#include "RangedWeapon.h"
#include "MyProjectGameMode.h"
#include "MyProjectPlayerController.h"
#include "Projectile.h"
#include "UserInterface.h"

#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"

#include "Camera/CameraComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "UObject/ConstructorHelpers.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Engine/GameInstance.h"
#include "Engine/EngineTypes.h"

#include "Materials/Material.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

#include "Math/Axis.h" 
#include "Engine/World.h"



DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AMyProject2DCharacter

AMyProject2DCharacter::AMyProject2DCharacter(const FObjectInitializer& PCIP) : Super(PCIP)
{
    Health = MaxHealth;
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(28.0f);
	GetCapsuleComponent()->SetCapsuleRadius(28.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(-90.0f, -90.0f, 0.0f));
    GetSprite()->SetRelativeRotation(FRotator(0.f,0.f,-90.f));

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
    SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
    SideViewCameraComponent->OrthoWidth = 1024.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    if (RootComponent == nullptr)
    {
        RootComponent = PCIP.CreateDefaultSubobject<USceneComponent>(this,TEXT("Root"));
    }
	// Configure character movement
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;
    GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
    /*GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, -1.0f));*/

    CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
    CursorToWorld->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
    if (DecalMaterialAsset.Succeeded())
    {
        CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
    }
    CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
    CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}
void AMyProject2DCharacter::BeginPlay()
{
    Super::BeginPlay();
    IncreasePowerBarDelegate.BindUObject(this,&AMyProject2DCharacter::IncreasePowerBar);
    if (ARangedWeapon* Weapon = GetWorld()->SpawnActor<ARangedWeapon>(StartingWeaponClass))
    {
        RangedWeapon = Weapon;
        RangedWeapon->AttachToComponent(GetSprite(),FAttachmentTransformRules::SnapToTargetIncludingScale);
        RangedWeapon->SetActorRelativeLocation(FVector(-20.f,0.f,-8.f));
        RangedWeapon->SetActorRelativeRotation(FRotator(0.f,0.f,0.f));
        APlayerController* PC = Cast<APlayerController>(GetController());
        RangedWeapon->SetPC(PC);
    }
}
//////////////////////////////////////////////////////////////////////////
// Animation

void AMyProject2DCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();
    UPaperFlipbook* DesiredAnimation;
    
    float GunRotation = RangedWeapon->GetGunRotation().GetComponentForAxis(EAxis::Z);
    if (GunRotation < 90.f && GunRotation > -90.f)
    {
        DesiredAnimation = IdleFrontAnimation;
    }else
    {
        DesiredAnimation = IdleBackAnimation;
    }
	// Are we moving or standing still?
//	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleFrontAnimation;
    
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void AMyProject2DCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    
    if (CursorToWorld != nullptr)
    {
        if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
        {
            if (UWorld* World = GetWorld())
            {
                FHitResult HitResult;
                FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
                FVector StartLocation = SideViewCameraComponent->GetComponentLocation();
                FVector EndLocation = SideViewCameraComponent->GetComponentRotation().Vector() * 2000.0f;
                Params.AddIgnoredActor(this);
                World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
                FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
                CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
            }
        }
        else if (APlayerController* PC = Cast<APlayerController>(GetController()))
        {
            FHitResult TraceHitResult;
            PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
            FVector CursorFV = TraceHitResult.ImpactNormal;
            FRotator CursorR = CursorFV.Rotation();
            CursorToWorld->SetWorldLocation(TraceHitResult.Location);
            CursorToWorld->SetWorldRotation(CursorR);
        }
    }
    
    // Update animation to match the motion
    UpdateAnimation();
    if(PowerBar==100.f) Power();
}

void AMyProject2DCharacter::OnFire()
{
    RangedWeapon->OnFire(IncreasePowerBarDelegate);
}
//////////////////////////////////////////////////////////////////////////
// Input

void AMyProject2DCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProject2DCharacter::MoveRight);
    PlayerInputComponent->BindAxis("MoveUp", this, &AMyProject2DCharacter::MoveUp);
    
    
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyProject2DCharacter::OnFire);

}

void AMyProject2DCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AMyProject2DCharacter::MoveUp(float Value)
{
    /*UpdateChar();*/

    // Apply the input to the character motion
    AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
}



void AMyProject2DCharacter::Hit(AEnnemyBase* ennemy)
{
    if (bCanTakeDamage)
    {
        UGameInstance* GI = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        if(GI)
        {
            DecrementHealth(ennemy->DamageValue);
            DecreasePowerBar();
            bCanTakeDamage = false;
            if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandler))
            {
                GetWorldTimerManager().SetTimer(TimerHandler, this, &AMyProject2DCharacter::BecomeVulnerable,3.f,false);
            }
        }
    }
}

void AMyProject2DCharacter::DecreasePowerBar()
{
    PowerBar = PowerBar >= 10.f ? PowerBar - 10.f : 0.f;
}
void AMyProject2DCharacter::IncreasePowerBar()
{
    PowerBar = PowerBar < 100.f ? PowerBar += 1.f : 100.f;
}

void AMyProject2DCharacter::Power()
{
}

void AMyProject2DCharacter::BecomeVulnerable()
{
    bCanTakeDamage = true;
}

void AMyProject2DCharacter::DecrementHealth(int damage)
{
    Health -= damage;
    if (Health <= 0.f)
    {
        Die();
    }
}
void AMyProject2DCharacter::Die()
{
}
