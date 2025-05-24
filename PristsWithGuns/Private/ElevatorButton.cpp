// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator/ElevatorButton.h"
#include <FMODSettings.h>

#include "ActorSequenceComponent.h"
#include "FMODBlueprintStatics.h"
#include "MyTweeningSubsystem.h"

// TODO make a method that waits for a delegate to unlock the button


AElevatorButton::AElevatorButton()
{

}

void AElevatorButton::BeginPlay()
{
    Super::BeginPlay();
}

void AElevatorButton::Interact(ACharacter *Character)
{
    if (!ButtonPressAnim || !ButtonMeshComponent || !PressedMaterial || !ErrorSound)
    {
        return;
    }
    
    ButtonMeshComponent->PlayAnimation(ButtonPressAnim, false);

    if(bIsUnlocked)
    {
        UFMODBlueprintStatics::PlayEventAttached(SucceessSound, ButtonMeshComponent, NAME_None, FVector::ZeroVector,
                                                 EAttachLocation::SnapToTarget, true, true, true);
        ButtonMeshComponent->SetMaterial(0, PressedMaterial);
    }
    else
    {
        
        UFMODBlueprintStatics::PlayEventAttached(ErrorSound, ButtonMeshComponent, NAME_None, FVector::ZeroVector,
                                                 EAttachLocation::SnapToTarget, true, true, true);
        
        // TODO set material to an ErrorMaterial
        
        OnButtonPressed.Broadcast();     
    }


}