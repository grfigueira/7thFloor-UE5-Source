// Fill out your copyright notice in the Description page of Project Settings.


#include "ReceptionBell.h"
#include "ActorSequenceComponent.h"
#include "FMODAudioComponent.h"

// Sets default values
AReceptionBell::AReceptionBell()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    
    FmodAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FmodAudioComponent"));
    FmodAudioComponent->SetupAttachment(Mesh);
    FmodAudioComponent->SetAutoActivate(false);
}

void AReceptionBell::BeginPlay()
{
    Super::BeginPlay();

    ButtonPressSequence = FindComponentByClass<UActorSequenceComponent>();
    if(ButtonPressSequence)
    {
        ButtonPressSequence->RegisterComponent();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Couldn't find actor sequence component"));
    }
    
}

void AReceptionBell::Interact(ACharacter *Character)
{
    if(FmodAudioComponent && ButtonPressSequence)
    {
        ButtonPressSequence->StopSequence();
        ButtonPressSequence->PlaySequence();
        FmodAudioComponent->Play();
    }
}

FText AReceptionBell::GetInteractionText() const
{
    return FText::FromString("Ring Bell");
}



