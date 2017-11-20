// Fill out your copyright notice in the Description page of Project Settings.

#include "MySceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"


// Sets default values for this component's properties
UMySceneComponent::UMySceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMySceneComponent::BeginPlay()
{
	Super::BeginPlay();

	//ADD
	ElapsedTime = 0;
}


// Called every frame
void UMySceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//ADD
	ElapsedTime += DeltaTime;
	this->SetRelativeLocation(FVector(sin(ElapsedTime), cos(ElapsedTime), 0.5f) * 50);
}

