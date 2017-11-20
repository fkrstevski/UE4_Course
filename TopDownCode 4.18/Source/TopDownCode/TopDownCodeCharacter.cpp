// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TopDownCodeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "TopDownCode.h"
#include "TopDownCodePlayerController.h"
#include "MySceneComponent.h"
#include "Components/StaticMeshComponent.h"

ATopDownCodeCharacter::ATopDownCodeCharacter()
	: m_health(0)
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
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

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

void ATopDownCodeCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATopDownCodeCharacter::Tick(float DeltaSeconds)
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
}

void ATopDownCodeCharacter::AddHealth(float amount)
{
	m_health += amount;
	UE_LOG(LogTopDownCode, Log, TEXT("New Health: %f"), m_health); 
	
	ATopDownCodePlayerController* pPlayerController = Cast<ATopDownCodePlayerController>(GetController());
	if (pPlayerController)
	{
		pPlayerController->OnCollectPowerup("Health");
	}
	else
	{
		UE_LOG(LogTopDownCode, Error, TEXT("Attempting to add health to a non player"));
	}


	if (amount > 0)
	{
		UE_LOG(LogTopDownCode, Log, TEXT("ADD COMPONENT"));

		UMySceneComponent* Orbiter = NewObject<UMySceneComponent>(this);
		Orbiter->SetupAttachment(RootComponent);
		Orbiter->RegisterComponentWithWorld(GetWorld());

		UStaticMeshComponent* Visual = NewObject<UStaticMeshComponent>(this);
		UStaticMesh* SphereVisualAsset = LoadObject<UStaticMesh>(GetOuter(), TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

		Visual->SetStaticMesh(SphereVisualAsset);
		Visual->SetWorldScale3D(FVector(0.1f));
		Visual->SetupAttachment(Orbiter);

		if (!Orbiter->IsRegistered())
		{
			Orbiter->RegisterComponent();
		}
		if (!Visual->IsRegistered())
		{
			Visual->RegisterComponent();
		}

		ComponentArray.Add(Orbiter);
	}
	else
	{

		if (ComponentArray.Num() > 0)
		{
			UMySceneComponent* Orbiter = ComponentArray.Pop();
			if (Orbiter != nullptr)
			{
				UE_LOG(LogTopDownCode, Log, TEXT("REMOVE COMPONENT"));

				USceneComponent* Visual = Orbiter->GetChildComponent(0);
				if (Visual != nullptr)
				{
					Visual->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
					Visual->UnregisterComponent();
					Visual->DestroyComponent();
					Visual = nullptr;
				}

				Orbiter->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
				Orbiter->UnregisterComponent();
				Orbiter->DestroyComponent();
				Orbiter = nullptr;
			}
		}
	}
}
