// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraActor.h"
#include "UObject/UObjectGlobals.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

// Sets default values
AHeroCharacter::AHeroCharacter(const FObjectInitializer& ObjectInitializer)
{
	MyNumber = 0;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//This property by default has true value for the character
	//But in our displacement model we don�t want that the character rotates based in the controller rotation.
	bUseControllerRotationYaw = false;

	// CharacterMovement component configuration

	//This way we enable that the character rotates in the movement direction when the movement starts
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//Rotation factor to the previous property
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	//We decrease the MaxWalkSpeed default value so the character walks slowly.
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	//Initializing the USpringArmComponent attribute
	SpringArm = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));

	//Adding the springArm to the Character's RootComponent (the collision capsule)
	SpringArm->AttachTo(RootComponent);

	//bAbsoluteRotation allows us to define if this support will rotate with the player or will stay fixed.
	//In our case we don't want it to rotate with the character.
	SpringArm->bAbsoluteRotation = true;

	//The distance between the arm and its target. This value defines the distance between the character and the camera.
	//Try out different values to see the outcome.
	SpringArm->TargetArmLength = 503.f;

	//Socket Offset.
	//Socket is an anchor point to other components.
	//For instance, in the character case we can define the socket in the character's hand
	//this way we can add another component(for example a gun).
	//But in our case we will add a camera to our SpringArm
	SpringArm->SocketOffset = FVector(0.f, 0.f, 75.f);

	//The relative rotation of the arm regard to its parent.
	//We want the camera rotated 180 degrees in the Y axis in order to be situated parallel to the character, at the same level.
	//This way we achieve the classic side-scroller camera's style.
	SpringArm->RelativeRotation = FRotator(0.f, 0.f, 90.f); 

	// Creating the UCameraComponent instance.
	SideViewCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("SideViewCamera"));

	//The AttachTo method allow us to add an object to another in a given socket. It receives two parameters,
	//the first one, the object where we will be anchored (the springArm) and the second the socket's name where we will be anchored.
	//USpringArmComponent's SocketName returns the name of the components socket.
	SideViewCamera->AttachTo(SpringArm, USpringArmComponent::SocketName);

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

	static const FString ScrollingMessage(TEXT("Hello World: "));

	if (GEngine)
	{
		const int32 AlwaysAddKey = -1; // Passing -1 means that we will not try and overwrite an   
									   // existing message, just add a new one  
		//GEngine->AddOnScreenDebugMessage(AlwaysAddKey, 0.5f, FColor::Yellow, ScrollingMessage + FString::FromInt(MyNumber));

		const int32 MyNumberKey = 0; // Not passing -1 so each time through we will update the existing message instead  
									 // of making a new one  
		GEngine->AddOnScreenDebugMessage(MyNumberKey, 5.f, FColor::Yellow, FString::FromInt(MyNumber));

		++MyNumber; // Increase MyNumber so we can see it change on screen  
	}

}

// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Informs the engine that when the MoveRight entry is detected call AHeroCharacter:: MoveRight method
	InputComponent->BindAxis("MoveRight", this, &AHeroCharacter::MoveRight);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	//The Run entry is detected, when the Shift key is pressed we set that the ToggleRunState method should be call.
	InputComponent->BindAction("Run", IE_Pressed, this, &AHeroCharacter::ToggleRunState);

	//The Run entry is detected, when the Shift key is released we set that the ToggleRunState method should be call.
	InputComponent->BindAction("Run", IE_Released, this, &AHeroCharacter::ToggleRunState);

}

/**
*   It gets called when the MoveForward entry is detected (When the keys W or S are pressed)
*  @param Value is equal to 1 when D is detected and to -1 when A is detected.
*/
void AHeroCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// Adds a new movement to the right or left according to the Value value.
		AddMovementInput(FVector(-1.f, 0.f, 0.f), Value);
	}
}

/**
* Gets call when the engine detects the Run entry
* Change to the character run state.
*/
void AHeroCharacter::ToggleRunState()
{
	//If the CharacterMovement's MaxWalkSpeed attribute is 400.f we increase it to 900.f to achieve that the character will  move faster.
	//Otherwise we set it to 400 again so the character move in the walk speed range.
	if (GetCharacterMovement()->MaxWalkSpeed == 400.0f)
		GetCharacterMovement()->MaxWalkSpeed = 900.0f;
	else
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}