// Fill out your copyright notice in the Description page of Project Settings.

#include "ObservableObject.h"
#include "CrosshairWidget.h"

#include "PristsWithGuns/PristsWithGunsCharacter.h"

// Sets default values
AObservableObject::AObservableObject()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    FmodAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Typing Sound FMOD Component"));
    FmodAudioComponent->SetupAttachment(Mesh);
    FmodAudioComponent->SetAutoActivate(false);

    CurrentStringIndex = 0;
    DelayAccumulator = 0;
}

// Called when the game starts or when spawned
void AObservableObject::BeginPlay() { Super::BeginPlay(); }

void AObservableObject::Interact(ACharacter *Character)
{
    if (bIsObsTyping)
    {
        return;
    }

    if (APristsWithGunsCharacter *ActualCharacter = Cast<APristsWithGunsCharacter>(Character))
    {
        CrosshairWidget = ActualCharacter->CrosshairWidget;
    }

    TempObsString = Observation;
    bIsObsTyping = true;
    if (FmodAudioComponent)
    {
        FmodAudioComponent->Play();
    }
}

FText AObservableObject::GetInteractionText() const { 
    return FText::FromString("Inspect"); 
}

// Called every frame
void AObservableObject::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsObsTyping)
    {

        if (!CrosshairWidget)
        {
            UE_LOG(LogTemp, Error, TEXT("CrosshairWidget is null! ObservableObject is trying to use it!"));
        }

        if (Speed == 0)
        {
            UE_LOG(LogTemp, Error, TEXT("Speed can't be zero!!"));
        }

        DelayAccumulator += DeltaTime;

        if (DelayAccumulator >= 1 / Speed)
        {

            CrosshairWidget->SetInteractionText(
                FText::FromString(TempObsString.LeftChop(TempObsString.Len() - CurrentStringIndex)));
            CurrentStringIndex++;

            if (CurrentStringIndex > Observation.Len())
            {
                bIsObsTyping = false;
                CurrentStringIndex = 0;
                FmodAudioComponent->Stop();
            }
            DelayAccumulator = 0;
        }
    }
}