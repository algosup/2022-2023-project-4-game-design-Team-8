// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject2DCharacter.h"
#include "PaperFlipbookComponent.h"
#include "RangedWeapon.h"
#include "MyProjectGameMode.h"
#include "MyProjectPlayerController.h"
#include "Projectile.h"
#include "UserInterface.h"
#include "PickableWeapon.h"
#include "PickableItem.h"

#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"

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

#include "PaperTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileLayer.h"
#include "PaperTileMap.h"

#include "Math/Axis.h" 
#include "Engine/World.h"



DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AMyProject2DCharacter

AMyProject2DCharacter::AMyProject2DCharacter()
{
    Health = MaxHealth;
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(13.0f);
	GetCapsuleComponent()->SetCapsuleRadius(13.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(-90.0f, -90.0f, 0.0f));
    GetSprite()->SetRelativeRotation(FRotator(0.f,0.f,-90.f));
    GetSprite()->SetRelativeLocation(FVector(0.f,-9.f,0.f));
    GetCapsuleComponent()->BodyInstance.bLockXRotation = true;
    GetCapsuleComponent()->BodyInstance.bLockYRotation = true;
    
    
    
	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
    SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
    SideViewCameraComponent->OrthoWidth = 1024.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    
	// Configure character movement
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
    /*GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, -1.0f));*/
    IncreasePowerBarDelegate.BindUObject(this,&AMyProject2DCharacter::IncreasePowerBar);
    
    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}
void AMyProject2DCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (ARangedWeapon* Weapon = GetWorld()->SpawnActor<ARangedWeapon>(StartingWeaponClass))
    {
        InitWeapon(Weapon);
    }
    GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMyProject2DCharacter::OnHit);
}
void AMyProject2DCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (GetbIsFiring())
    {
        Fire();
    }
    // Update animation to match the motion
    UpdateAnimation();
}
//////////////////////////////////////////////////////////////////////////
// Animation

void AMyProject2DCharacter::InitWeapon(ARangedWeapon* Weapon)
{
    RangedWeapon = Weapon;
    RangedWeapon->SetOwner(this);
    RangedWeapon->AttachToComponent(GetSprite(),FAttachmentTransformRules::SnapToTargetIncludingScale);
    RangedWeapon->SetActorRelativeLocation(FVector(-20.f,0.f,-8.f));
    RangedWeapon->SetActorRelativeRotation(FRotator(0.f,0.f,0.f));
    APlayerController* PC = Cast<APlayerController>(GetController());
    RangedWeapon->SetPC(PC);
}
void AMyProject2DCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (APaperTileMapActor* Tile = Cast<APaperTileMapActor>(OtherActor))
    {
        FVector PlayerPosition = GetActorLocation();
        AMyProjectGameMode* GameMode = (AMyProjectGameMode*)GetWorld()->GetAuthGameMode();
        GameMode->OpenDoor(PlayerPosition,Tile,GetCharacterMovement()->GetLastInputVector());
    }
    else if (APickableItem* PickedItem = Cast<APickableItem>(OtherActor))
    {

    }
    else if (APickableWeapon* PickableWeapon = Cast<APickableWeapon>(OtherActor))
    {
        AMyProjectGameMode* GameMode = (AMyProjectGameMode*)GetWorld()->GetAuthGameMode();
        RangedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        GameMode->DropWeapon(RangedWeapon, PickableWeapon->GetActorLocation());
        ARangedWeapon* PickedWeapon = PickableWeapon->RangedWeapon;
        FDetachmentTransformRules rules = FDetachmentTransformRules(EDetachmentRule::KeepRelative,true);
        PickedWeapon->DetachFromActor(rules);
        InitWeapon(PickedWeapon);
        PickableWeapon->PickedUp();
        LaunchCharacter((-GetCharacterMovement()->GetLastInputVector()) * 200, true, true);
    }
}

void AMyProject2DCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();
    UPaperFlipbook* DesiredAnimation;
    
    float GunRotation = RangedWeapon->GetGunRotation().GetComponentForAxis(EAxis::Z);
    if (GunRotation <= 0.f)
    {
        DesiredAnimation = IdleBackAnimation;
    }else
    {
        DesiredAnimation = IdleFrontAnimation;
    }
	// Are we moving or standing still?
//	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleFrontAnimation;
    
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProject2DCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProject2DCharacter::MoveRight);
    PlayerInputComponent->BindAxis("MoveUp", this, &AMyProject2DCharacter::MoveUp);
    
    
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyProject2DCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyProject2DCharacter::StopFire);

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


void AMyProject2DCharacter::StartFire()
{
    bIsFiring = true;
}
void AMyProject2DCharacter::StopFire()
{
    bIsFiring = false;
}
void AMyProject2DCharacter::Fire()
{
    RangedWeapon->OnFire(IncreasePowerBarDelegate);
}

void AMyProject2DCharacter::Hit(AEnnemyBase* ennemy)
{
    if (GetbCanTakeDamage())
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
    if (GetPowerBar() == 100.f) UnPower();
    PowerBar = GetPowerBar() >= 10.f ? GetPowerBar() - 10.f : 0.f;
}
void AMyProject2DCharacter::IncreasePowerBar()
{
    PowerBar = GetPowerBar() < 100.f ? GetPowerBar() + (1.f * PowerBarMultiplier) : 100.f;
    if (GetPowerBar() == 100.f) Power();
}

void AMyProject2DCharacter::Power()
{
}
void AMyProject2DCharacter::UnPower()
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
    AMyProjectGameMode* GameMode = (AMyProjectGameMode*)GetWorld()->GetAuthGameMode();
    GameMode->RestartGame();
}
