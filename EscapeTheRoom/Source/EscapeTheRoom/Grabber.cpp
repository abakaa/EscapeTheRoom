// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UWorld* world = GetWorld();
    FVector location;
    FRotator rotation;
    world->GetFirstPlayerController()->GetPlayerViewPoint(OUT location, OUT rotation);
    
    ///Logging
    UE_LOG(LogTemp, Display, TEXT("Position : %s\nDirection : %s"), *location.ToString(), *rotation.ToString());
    
    ///Draw a red line
    FVector lineTraceEnd = location + rotation.Vector() * _reach;
    DrawDebugLine(world, location, lineTraceEnd, FColor(255,0,0),false, 0.f, 0.f, 10.f);
    
    ///Line-Trace to reach distance
    FHitResult hit;
    FCollisionQueryParams TraceParams(FName(TEXT("")),false, GetOwner());
    
    world->LineTraceSingleByObjectType(OUT hit,
                                       location,
                                       lineTraceEnd,
                                       FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                       FCollisionQueryParams::DefaultQueryParam);
    
    //See what i hit
    AActor* actorHit = hit.GetActor();
    if(actorHit){
        UE_LOG(LogTemp, Warning, TEXT("Actor Hit : %s"), *actorHit->GetName())
    }
}

