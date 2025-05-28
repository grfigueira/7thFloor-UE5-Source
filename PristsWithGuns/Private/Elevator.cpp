// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator/Elevator.h"

AElevator::AElevator()
{
    PrimaryActorTick.bCanEverTick = true;

    Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
    RootComponent = Frame;

    DoorLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLeft"));
    DoorRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorRight"));

    DoorLeft->SetupAttachment(Frame);
    DoorRight->SetupAttachment(Frame);

}

void AElevator::BeginPlay()
{
    Super::BeginPlay();
}


void AElevator::OpenDoors()
{
    LeftTargetPos = DoorLeft->GetRelativeLocation();
    LeftTargetPos.X -= DoorOpenedDistance;

    RightTargetPos = DoorRight->GetRelativeLocation();
    RightTargetPos.X += DoorOpenedDistance;
    
    bOpeningDoors = true;
}

void AElevator::CloseDoors()
{
    LeftTargetPos = DoorLeft->GetRelativeLocation();
    LeftTargetPos.X += DoorOpenedDistance;

    RightTargetPos = DoorRight->GetRelativeLocation();
    RightTargetPos.X -= DoorOpenedDistance;
    
    bOpeningDoors = true;
}


void AElevator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // == Opening left door ==

    if (bOpeningDoors)
    {
        OpenElevatorDoor(DoorRight, RightTargetPos, bOpeningDoors, DeltaTime);
        OpenElevatorDoor(DoorLeft, LeftTargetPos, bOpeningDoors, DeltaTime);
    }
}

void AElevator::OpenElevatorDoor(const TObjectPtr<UStaticMeshComponent> &Door, const FVector &TargetPos,
                                 bool &IsOpeningDoors, const float DeltaTime)
{
    const FVector DoorNewPos = FMath::VInterpTo(Door->GetRelativeLocation(), TargetPos, DeltaTime,
                                                DoorSpeed);
    Door->SetRelativeLocation(DoorNewPos);

    UE_LOG(LogTemp, Warning, TEXT("Here the difference: %f"), FMath::Abs(DoorNewPos.X - TargetPos.X));

    if (FMath::Abs(DoorNewPos.X - TargetPos.X) < 10.0f) // Close enough to finish
    {
        UE_LOG(LogTemp, Warning, TEXT("bOpeningDoors was set to false"));
        IsOpeningDoors = false;
    }

}