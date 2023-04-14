// Fill out your copyright notice in the Description page of Project Settings.


#include "EnnemyBase.h"
#include "MyProject2DCharacter.h"
#include "Projectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyProjectGameMode.h"
// Sets default values
AEnnemyBase::AEnnemyBase()
{
    Health = MaxHealth;
    PrimaryActorTick.bCanEverTick = true;
    //CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
    GetCapsuleComponent()->SetCapsuleHalfHeight(28.0f);
    GetCapsuleComponent()->SetCapsuleRadius(28.0f);
    GetCapsuleComponent()->BodyInstance.bLockXRotation = true;
    GetCapsuleComponent()->BodyInstance.bLockYRotation = true;
    
    /*GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, -1.0f));*/
    GetSprite()->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
//    GetSprite()->SetRelativeLocation(FVector(0.f,-9.f,0.f));
}
// Called when the game starts or when spawned
void AEnnemyBase::BeginPlay()
{
    Super::BeginPlay();

    CapsuleComp = GetCapsuleComponent();
    //CapsuleComp->SetSimulatePhysics(true);
//    RootComponent->SetRelativeScale3D(FVector(0.4f,0.4f,0.4f));
    CapsuleComp->OnComponentHit.AddDynamic(this, &AEnnemyBase::OnHit);
}

// Called every frame
void AEnnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    APawn* Character = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
    FVector Destination = (Character->GetActorLocation() - GetActorLocation());
    AddMovementInput(Destination);
    //GetCharacterMovement()->Velocity = GetCharacterMovement()->Velocity * 5;
    UPaperFlipbook* DesiredAnimation;
    if(abs(GetCharacterMovement()->Velocity.X) > abs(GetCharacterMovement()->Velocity.Y))
    {
        if(GetCharacterMovement()->Velocity.X < 0)
        {
            DesiredAnimation = RunningLeftAnimation;
        }
        else
        {
            DesiredAnimation = RunningRightAnimation;
        }
    }
    else 
    {
        if(GetCharacterMovement()->Velocity.Y < 0)
        {
            DesiredAnimation = RunningBackAnimation;
        }
        else
        {
            DesiredAnimation = RunningFrontAnimation;
        }
    }
    if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}
void AEnnemyBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (AMyProject2DCharacter* player = Cast<AMyProject2DCharacter>(OtherActor))
    {
        player->Hit(this);
    }
}

void AEnnemyBase::Hit(AActorToSpawn* projectile)
{
    DecrementHealth(projectile->DamageValue);
    GetSprite()->SetSpriteColor(FLinearColor::Red);
    if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandler))
    {
        GetWorldTimerManager().SetTimer(TimerHandler, this, &AEnnemyBase::ResetColor, 0.5f, false);

    }
}

void AEnnemyBase::ResetColor()
{
    GetSprite()->SetSpriteColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
}

void AEnnemyBase::DecrementHealth(int damage)
{
    Health -= damage;

    if(Health <= 0.f)
    {
        Die();
    }
}
void AEnnemyBase::Die()
{
    AMyProjectGameMode* GameMode = (AMyProjectGameMode*)GetWorld()->GetAuthGameMode();
    GameMode->EnnemyDeath();
    Destroy();
}
