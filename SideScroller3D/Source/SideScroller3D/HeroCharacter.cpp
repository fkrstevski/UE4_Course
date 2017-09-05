// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AHeroCharacter::AHeroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//This property by default has true value for the character
	//But in our displacement model we don’t want that the character rotates based in the controller rotation.
	bUseControllerRotationYaw = false;

	// CharacterMovement component configuration

	//This way we enable that the character rotates in the movement direction when the movement starts
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//Rotation factor to the previous property
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	//We decrease the MaxWalkSpeed default value so the character walks slowly.
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Informs the engine that when the MoveForward entry is detected call AHeroCharacter::MoveForward method
	InputComponent->BindAxis("MoveForward", this, &AHeroCharacter::MoveForward);

	// Informs the engine that when the MoveRight entry is detected call AHeroCharacter:: MoveRight method
	InputComponent->BindAxis("MoveRight", this, &AHeroCharacter::MoveRight);
}

/**
*   It gets called when the MoveForward entry is detected (When the keys W or S are pressed)
*  Calculates the direction of the character and applies it a movement (positive or negative) in that direction.
*
*  @param Value is equal to 1 when W is pressed and -1 when S is.
*/
void AHeroCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		//Gets the current rotation
		const FRotator Rotation = Controller->GetControlRotation();

		// Creates the direction vector and applies the movement
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

/**
*   It gets called when the MoveForward entry is detected (When the keys W or S are pressed)
*  @param Value is equal to 1 when D is detected and to -1 when A is detected.
*/
void AHeroCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		//Determines the direction of the side movements. Notice that we only have interest in the rotation in the Y axis.
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Creates the direction vector and applies the movement
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

