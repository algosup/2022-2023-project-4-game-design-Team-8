// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject2DCharacter.h"
#include "PaperFlipbookComponent.h"
#include "RangedWeapon.h"
#include "MyProjectGameMode.h"
#include "MyProjectPlayerController.h"
#include "ActorToSpawn.h"

#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/DecalComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"

#include "Camera/CameraComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "UObject/ConstructorHelpers.h"

#include "HeadMountedDisplayFunctionLibrary.h"

#include "Materials/Material.h"
#include "DrawDebugHelpers.h"

#include "Engine/World.h"



DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AMyProject2DCharacter

AMyProject2DCharacter::AMyProject2DCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
//	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(-50.0f, -90.0f, 0.0f));
    GetSprite()->SetRelativeRotation(FRotator(0.f,0.f,0.f));

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
//	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Perspective;
//	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
    
    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
    MuzzleLocation->SetupAttachment(GunMesh);
    MuzzleLocation->SetRelativeLocation(FVector(0.0f, 56.5f, 11.3f));
	// Prevent all automatic rotation behavior on the camera, character, and camera component
//	CameraBoom->SetUsingAbsoluteRotation(true);
//	SideViewCameraComponent->bUsePawnControlRotation = false;
//	SideViewCameraComponent->bAutoActivate = true;
//	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
//	GetCharacterMovement()->bConstrainToPlane = true;
//	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

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

    GunMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}
//////////////////////////////////////////////////////////////////////////
// Animation

void AMyProject2DCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void AMyProject2DCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();
    
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

    // Now setup the rotation of the controller based on the direction we are travelling
    const FVector PlayerVelocity = GetVelocity();
    float TravelDirection = PlayerVelocity.X;
    
    // Declaration of variables to hold mouse vectors.
    FVector MouseDir = FVector::ZeroVector;
    FVector MousePos = FVector::ZeroVector;
    // Pass by reference to get mouse position in world space and direction vector.
    APlayerController* PC = Cast<APlayerController>(GetController());
    PC->DeprojectMousePositionToWorld(MousePos, MouseDir);

    // Declaration of vector of intersection.
    FVector Intersection = FVector::ZeroVector;
    float t = 0.f;
    // Vector from camera that crosses the plane we want the intersection.
    FVector LineEnd = MousePos + MouseDir * 2000.f;
    // Get intersection vector. Returns true if intersection was possible.
    bool bIntersectionSuccess = UKismetMathLibrary::LinePlaneIntersection_OriginNormal(
        MousePos,
        LineEnd,
        GetActorLocation(),
        GetActorUpVector(),
        t,
        Intersection
    );
    // Do stuff if line intersected.
    if (bIntersectionSuccess)
    {
        // Calculate direction vector from the pawn body forward vector to intersection vector.
        FVector DirToIntersection = (Intersection -  GunMesh->GetComponentLocation()).GetSafeNormal();
        // Gets the cosine of the angle between the pawns body forward vector and the direction to intersection.
        float dotForward = GunMesh->GetRightVector() | DirToIntersection;
        // Converts the cosine of the angle to degrees.
        float Angle = acos(dotForward) * (180.f / PI);
        // Only update the rotation if it is greater to avoid unwanted behaviour.
//        if (Angle > .2f)
//        {
            // Clamp to limit how fast the component can rotate.
            Angle = FMath::Clamp(Angle, 0.f, 25.f);
            // Gets the cosine of the angle with the right vector against direction to intersection to know on what side of the component is the intersection.
        float dotSide = GunMesh->GetForwardVector() | DirToIntersection;
            // Negates the value depending on what side is the intersection relative to the component.
            Angle *= 10.f * ((dotSide < 0.f) ? 1.f : -1.f);
            // Create rotator with variable.
        float BodyRotator = Angle * DeltaSeconds;
        GunMesh->AddRelativeRotation(FRotator(0.f,BodyRotator,0.f));
//         Set the rotation so that the character faces his direction of travel.
//        if (Controller != nullptr)
//        {
//            if (BodyRotator < 0.0f)
//            {
//    //  GetSprite()->SetRelativeRotation(FRotator(0.f,90.f,30.f));
//                Controller->SetControlRotation(FRotator(0.0, 0.0f, 0.0f));
//            }
//            else if (BodyRotator > 0.0f)
//            {
//    //  GetSprite()->SetRelativeRotation(FRotator(0.f,90.f,-30.f));
//                Controller->SetControlRotation(FRotator(0.0f, 180.0f, 0.0f));
//            }
//        }
        DrawDebugLine(GetWorld(), GetActorLocation(), Intersection, FColor::Orange, false, -1.f, 0, 4.f);
                DrawDebugSphere(GetWorld(), Intersection, 10.f, 16, FColor::Red, false);
//        }
    }
}

void AMyProject2DCharacter::OnFire()
{
    if(GetWorld() != NULL)
    {
        SpawnRotation = GunMesh->GetComponentRotation();

        SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) +SpawnRotation.RotateVector(GunOffset);

        FActorSpawnParameters ActorSpawnParams;
        ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        GetWorld()->SpawnActor<AActorToSpawn>(Projectile, SpawnLocation, SpawnRotation, ActorSpawnParams);
    }
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

void AMyProject2DCharacter::UpdateCharacter()
{
	
    
	
}
