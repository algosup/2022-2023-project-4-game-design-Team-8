// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"

#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "RangedWeapon.h"
#include "Projectile.h"
#include "MyProjectGameMode.h"
#include "MyProjectPlayerController.h"


AMyProjectCharacter::AMyProjectCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = true; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	CharacterMesh->SetupAttachment(GetCapsuleComponent());
	CharacterMesh->AddRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	CharacterMesh->AddRelativeLocation(FVector(0.0f, 0.0f, -88.0f));

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(GunMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.0f, 56.5f, 11.3f));
	//GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Create a decal in the world to show the cursor's location
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
void AMyProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	GunMesh->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));

}
void AMyProjectCharacter::Tick(float DeltaSeconds)
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
                FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
                FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
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
        FVector DirToIntersection = (Intersection - CharacterMesh->GetComponentLocation()).GetSafeNormal();
        // Gets the cosine of the angle between the pawns body forward vector and the direction to intersection.
        float dotForward = CharacterMesh->GetForwardVector() | DirToIntersection;
        // Converts the cosine of the angle to degrees.
        float Angle = acos(dotForward) * (180.f / PI);
        // Only update the rotation if it is greater to avoid unwanted behaviour.
//        if (Angle > .2f)
//        {
            // Clamp to limit how fast the component can rotate.
            Angle = FMath::Clamp(Angle, 0.f, 25.f);
            // Gets the cosine of the angle with the right vector against direction to intersection to know on what side of the component is the intersection.
            float dotSide = CharacterMesh->GetComponentLocation() | DirToIntersection;
            // Negates the value depending on what side is the intersection relative to the component.
            Angle *= 10.f * ((dotSide > 0.f) ? 1.f : -1.f);
            // Create rotator with variable.
            FRotator BodyRotator = FRotator(0.f,Angle * DeltaSeconds, 0.f);
            // Add rotation to pawn body component.
        
            CharacterMesh->AddRelativeRotation(BodyRotator);
//        }
    }
}


//////////////////////////////////////////////////////////////////////////
// Input

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyProjectCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyProjectCharacter::OnResetVR);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyProjectCharacter::OnFire);
}


void AMyProjectCharacter::OnResetVR()
{
	// If MyProject2 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in MyProject2.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AMyProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AMyProjectCharacter::OnFire()
{
	if(GetWorld() != NULL)
	{
		SpawnRotation = CharacterMesh->GetComponentRotation();

		SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) +SpawnRotation.RotateVector(GunOffset);

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		GetWorld()->SpawnActor<AProjectile>(Projectile, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}
}

void AMyProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyProjectCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
