// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator/ElevatorButton.h"
#include <FMODSettings.h>

#include "ActorSequenceComponent.h"
#include "FMODBlueprintStatics.h"
#include "Engine/PointLight.h"

// TODO make a method that waits for a delegate to unlock the button


AElevatorButton::AElevatorButton()
{

}


void AElevatorButton::BeginPlay()
{
    Super::Super::BeginPlay();
}

void AElevatorButton::Interact(ACharacter *Character)
{
    if (!ButtonMeshComponent)
    {
        return;
    }
    
    TryToUseLockedButton(); // Implemented in blueprint

    if(bIsUnlocked)
    {
        OnButtonPressed.Broadcast();
    }

}

bool AElevatorButton::UseKey(int16 InKeyId)
{
    bIsUnlocked = ButtonKeyId == InKeyId;
    return bIsUnlocked;
}

void AElevatorButton::LockButton()
{
    bIsUnlocked = false;
}

void AElevatorButton::UnlockButton()
{
    bIsUnlocked = true;
}