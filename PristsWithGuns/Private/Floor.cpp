// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"

#include "Engine/PointLight.h"


AFloor::AFloor()
{

}

void AFloor::BeginPlay()
{
    Super::BeginPlay();
    SetActorHiddenInGame(!bIsActive);

    InitializeAttachedLightsArray();

    LocActive = GetTransform().GetLocation();
}

// Also already sets the lights on/off depending on bIsActive
void AFloor::InitializeAttachedLightsArray()
{
    TArray<AActor *> AttachedActors;
    GetAttachedActors(AttachedActors);
    for (AActor *AttachedActor : AttachedActors)
    {
        if (APointLight *PointLight = Cast<APointLight>(AttachedActor))
        {
            AttachedLights.Add(PointLight);
            PointLight->SetEnabled(bIsActive);
        }
    }
}

void AFloor::SetIsActive(bool bValue)
{
    bIsActive = bValue;
    SetActorHiddenInGame(!bValue);
}

void AFloor::BringToElevator()
{
    if (!RootComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("No root component found (?)"));
        return;
    }

    SetIsActive(true);

    RootComponent->SetMobility(EComponentMobility::Movable);
    SetActorLocation(LocActive);
    RootComponent->SetMobility(EComponentMobility::Type::Static);
    SetPointLightsEnabled(true);
}

void AFloor::MoveAway()
{

    if (!RootComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("No root component found (?)"));
        return;
    }

    SetIsActive(false);

    FVector AwayLocation = LocActive;
    AwayLocation.Z = ZWhenAway;
    RootComponent->SetMobility(EComponentMobility::Movable);
    SetActorLocation(AwayLocation);
    RootComponent->SetMobility(EComponentMobility::Type::Static);
    SetPointLightsEnabled(false);

}

void AFloor::SetPointLightsEnabled(bool bNewEnabled)
{
    for(APointLight* PointLight : AttachedLights)
    {
        PointLight->SetEnabled(bNewEnabled);
    }
}
