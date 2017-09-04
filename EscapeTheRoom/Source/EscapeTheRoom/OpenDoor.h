// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPETHEROOM_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY(EditAnywhere)
    float _openAngle = 90.f;
    
    UPROPERTY(EditAnywhere)
    ATriggerVolume* _pressurePlate = nullptr;
    
    UPROPERTY(EditAnywhere)
    float _doorCloseDelay = 1.f;
    
    float _lastOpenDoorTime;
    float _minPressure = 20.f;
    
    AActor* _owner = nullptr; //the owning door
    void OpenDoor();
    void CloseDoor();
    
    float GetTotalMassOnPressurePlate();
    
    UPROPERTY(BlueprintAssignable)
    FOnOpenRequest OpenRequested;
};
