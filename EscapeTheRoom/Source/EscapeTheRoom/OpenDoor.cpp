// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    UWorld* world = GetWorld();
    _owner = GetOwner();
}

void UOpenDoor::OpenDoor(){
    OpenRequested.Broadcast();
}

void UOpenDoor::CloseDoor(){
    AActor* owner = GetOwner();
    FRotator rotator = FRotator(0.f, 0.f, 0.f);
    
    owner->SetActorRotation(rotator);
}

float UOpenDoor::GetTotalMassOnPressurePlate(){
    float totalMass = 0.f;
    
    TArray<AActor*> overlappingActors;
    _pressurePlate->GetOverlappingActors(OUT overlappingActors);
    
    for (const auto* actor : overlappingActors) {
        auto primitive = actor->FindComponentByClass<UPrimitiveComponent>();
        totalMass += primitive->GetMass();
    }
    
    return totalMass;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UWorld* world = GetWorld();
    
    if(GetTotalMassOnPressurePlate() >= _minPressure){
        OpenDoor();
        _lastOpenDoorTime = world->GetTimeSeconds();
    }
    
    if(world->GetTimeSeconds() - _lastOpenDoorTime > _doorCloseDelay){
        CloseDoor();
    }
}

