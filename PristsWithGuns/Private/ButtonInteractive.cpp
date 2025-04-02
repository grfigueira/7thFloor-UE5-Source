// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonInteractive.h"

AButtonInteractive::AButtonInteractive()
{
	PrimaryActorTick.bCanEverTick = true;

        ButtonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ButtonMeshComponent"));
        RootComponent = ButtonMeshComponent;
}

void AButtonInteractive::BeginPlay()
{
	Super::BeginPlay();

    if(NeutralMaterial && ButtonMeshComponent)
    {
        ButtonMeshComponent->SetMaterial(0, NeutralMaterial);
    }
	
}

void AButtonInteractive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AButtonInteractive::Interact(ACharacter *Character)
{
    if(ButtonPressAnim && ButtonMeshComponent)
    {
        ButtonMeshComponent->PlayAnimation(ButtonPressAnim, false);
        if(PressedMaterial)
        {
            ButtonMeshComponent->SetMaterial(0, PressedMaterial);
        }
    }
    OnButtonPressed.Broadcast();
    
}

FText AButtonInteractive::GetInteractionText() const
{
    return FText::FromString("Press Button");
}

