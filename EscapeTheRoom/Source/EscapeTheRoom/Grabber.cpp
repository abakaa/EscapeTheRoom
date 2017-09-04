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

void UGrabber::Grab(){
    FHitResult hitResult = GetFirstPhysicsBodyInReach();
    auto comp = hitResult.GetComponent();
    AActor* actorhit = hitResult.GetActor();
    
    
    //Check if we hit something, then attach physics handle.
    if(actorhit != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Grabbed"))
        _physicsHandle->GrabComponentAtLocationWithRotation(comp,
                                                            EName::NAME_None,
                                                            actorhit->GetActorLocation(),
                                                            actorhit->GetActorRotation());
    }
}

void UGrabber::Release(){
    if(_physicsHandle->GrabbedComponent != nullptr){
        _physicsHandle->ReleaseComponent();
        UE_LOG(LogTemp, Warning, TEXT("Released"))
    }
    
}

void UGrabber::FindPhysicsHandleComponent(){
    AActor* owner = GetOwner();
    _physicsHandle = owner->FindComponentByClass<UPhysicsHandleComponent>();
    if(!_physicsHandle){
        UE_LOG(LogTemp, Error, TEXT("%s named object does not have physics handle."), *owner->GetName())
    }
}

void UGrabber::SetupInputComponent(){
    AActor* owner = GetOwner();
    _inputComponent = owner->FindComponentByClass<UInputComponent>();
    if(_inputComponent){
        _inputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
        _inputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
    }
    else{
        UE_LOG(LogTemp, Error, TEXT("%s named object does not have input component."), *owner->GetName())
    }
}

FVector UGrabber::GetLineTraceStart(){
    UWorld* world = GetWorld();
    FVector location;
    FRotator rotation;
    world->GetFirstPlayerController()->GetPlayerViewPoint(OUT location, OUT rotation);
    return location;
}

FVector UGrabber::GetLineTraceEnd(){
    UWorld* world = GetWorld();
    FVector location;
    FRotator rotation;
    world->GetFirstPlayerController()->GetPlayerViewPoint(OUT location, OUT rotation);
    
    FVector lineTraceEnd = location + rotation.Vector() * _reach;
    return lineTraceEnd;
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
    ///Line-Trace to reach distance
    UWorld* world = GetWorld();
    FHitResult hit;
    FCollisionQueryParams TraceParams(FName(TEXT("")),false, GetOwner());
    
    world->LineTraceSingleByObjectType(OUT hit,
                                       GetLineTraceStart(),
                                       GetLineTraceEnd(),
                                       FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                       TraceParams);
    
    return hit;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(_physicsHandle->GrabbedComponent){
        _physicsHandle->SetTargetLocation(GetLineTraceEnd());
    }
}

