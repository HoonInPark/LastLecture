// Copyright Epic Games, Inc. All Rights Reserved.

#include "LastLectureCharacter.h"
#include "LastLectureProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "LL_AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/StaticMeshActor.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ALastLectureCharacter

ALastLectureCharacter::ALastLectureCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	Box->SetupAttachment(GetCapsuleComponent());
	Box->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	Box->SetRelativeLocation(FVector(2.250000f, 3.250000f, 3.000000f));
}

void ALastLectureCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void ALastLectureCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALastLectureCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALastLectureCharacter::Look);

		// Pointing
		EnhancedInputComponent->BindAction(PointAction, ETriggerEvent::Triggered, this, &ALastLectureCharacter::Point);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}


void ALastLectureCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ALastLectureCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ALastLectureCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ALastLectureCharacter::GetHasRifle()
{
	return bHasRifle;
}

#pragma region LastLecture
void ALastLectureCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UAnimInstance* CurrentAnimInstance = GetMesh()->GetAnimInstance();
	if (!CurrentAnimInstance)
	{
		ThisAnimInstance = NewObject<ULL_AnimInstance>(GetMesh(), ULL_AnimInstance::StaticClass());
		GetMesh()->SetAnimInstanceClass(ThisAnimInstance->GetClass());
	}
	else
		ThisAnimInstance = CurrentAnimInstance;
}

void ALastLectureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALastLectureCharacter::Point(const FInputActionValue& Value)
{
	TArray<AActor*> OverlappingActors;
	// 여기 GetOverlappingActors 두번째 인수에 어떤 값을 넣어줘야 하는지 엔진 소스를 보고 설명하기
	Box->GetOverlappingActors(OverlappingActors, AStaticMeshActor::StaticClass()); 

	if (0 != OverlappingActors.Num())
	{
		FVector TargetLocation;
		float Distance = 100000000.f;

		for (const auto OverlappingActor : OverlappingActors)
		{
			const float Distance_temp = FVector::Dist(FirstPersonCameraComponent->GetComponentLocation(),
			                                          OverlappingActor->GetActorLocation());

			TargetLocation = Distance < Distance_temp ? TargetLocation : OverlappingActor->GetActorLocation();
			Distance = Distance < Distance_temp ? Distance : Distance_temp;
		}
		Execute_Point_Message(ThisAnimInstance, TargetLocation);
	}
}
#pragma endregion LastLecture
