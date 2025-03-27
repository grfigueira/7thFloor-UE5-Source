// Fill out your copyright notice in the Description page of Project Settings.

#include "ObservableObjectComponent.h"
#include "CrosshairWidget.h"
#include "FMODAudioComponent.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "PristsWithGuns/PristsWithGunsCharacter.h"

// Sets default values for this component's properties
UObservableObjectComponent::UObservableObjectComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
    // features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    CurrentStringIndex = 0;
    DelayAccumulator = 0;
    CurrentResponseIndex = 0;
    bComingFromResponse = false;
    Dialogue = {};
    JsonFileName = "";
}

void UObservableObjectComponent::BeginPlay()
{
    Super::BeginPlay();

    if(JsonFileName.IsEmpty())
    {
        return;
    }
    
    const FString JsonFilePath = (FPaths::ProjectDir() + "Content/Data/" + JsonFileName);

    bool bSuccess;
    Dialogue = FDialogueParser::LoadDialogueFromFile(JsonFilePath, bSuccess);

    if(Dialogue.Num() == 0 && bSuccess)
    {
        return;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Couldn't access/deserialize JSON dialogue file"));
    }

    if (AActor *Owner = GetOwner())
    {
        FmodAudioComponent = NewObject<UFMODAudioComponent>(Owner, TEXT("Typing sound FMOD"));
        if (FmodAudioComponent)
        {
            FmodAudioComponent->RegisterComponent();
            FmodAudioComponent->AttachToComponent(Owner->GetRootComponent(),
                                                  FAttachmentTransformRules::KeepRelativeTransform);
            FmodAudioComponent->SetAutoActivate(false);
            if (SpeechSoundEvent)
            {
                FmodAudioComponent->SetEvent(SpeechSoundEvent);
            }
            UE_LOG(LogTemp, Display, TEXT("FmodAudioComponent was created."));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("FmodAudioComponent is null!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Owner is null!"));
    }

    if (Dialogue.Num() == 0)
    {
        TempObsString = "This isn't relevant.";
    }
}

void UObservableObjectComponent::Interact(ACharacter *Character)
{
    if (bIsObsTyping)
    {
        return;
    }

    bIsInteracting = true;

    if (APristsWithGunsCharacter *ActualCharacter = Cast<APristsWithGunsCharacter>(Character))
    {
        CrosshairWidget = ActualCharacter->CrosshairWidget;
    }

    TempObsString = Dialogue[CurrentNodeId].Text;
    bIsObsTyping = true;
    DisplayResponseOptions();
    if (FmodAudioComponent)
    {
        FmodAudioComponent->Play();
    }
}

FText UObservableObjectComponent::GetInteractionText() const { 
    return FText::FromString(HoverPopupText);
}

void UObservableObjectComponent::DisplayResponseOptions()
{
    if (CurrentResponseIndex >= Dialogue[CurrentNodeId].Responses.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("All available responses have already been rendered"));
        CurrentResponseIndex = 0;
        return;
    }

    //-----------------------------------------
    //---- IF THERE ARE POSSIBLE RESPONSES ----
    //-----------------------------------------

    if (Dialogue[CurrentNodeId].Responses.Num() > 0)
    {
        
        if (APristsWithGunsCharacter *Character =
                Cast<APristsWithGunsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
        {

            if (APlayerController *PC = Cast<APlayerController>(Character->GetController()))
            {
                PC->SetIgnoreMoveInput(true);
                PC->SetIgnoreLookInput(true);

                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SETTING IGNORE MOVE AND LOOK TO TRUE"));

                PC->bShowMouseCursor = true;

                FInputModeUIOnly InputModeData;
                InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

                PC->SetInputMode(InputModeData);
            }
        }

        int ResponseNodeId = Dialogue[CurrentNodeId].Responses[CurrentResponseIndex];
        FDialogueNode ResponseNode = Dialogue[ResponseNodeId];

        CrosshairWidget->AddDialogueOption(
            ResponseNode.Text,
            [this, ResponseNode]()
            {
                CurrentNodeId = ResponseNode.Next_Node_ID;
                bComingFromResponse = true;
                NextObservation();
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("In Callback lambda function!!"));
                // TODO check for callback tags
                
            });

        CurrentResponseIndex++;

        // delay between responses
        GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UObservableObjectComponent::DisplayResponseOptions,
                                               0.27f, false);
    }
}

void UObservableObjectComponent::DisplayNextResponse() {}

void UObservableObjectComponent::NextObservation()
{
    if (!bIsObsTyping && CurrentNodeId != -1 && Dialogue[CurrentNodeId].Next_Node_ID != -1)
    {
        if (APristsWithGunsCharacter *Character =
                Cast<APristsWithGunsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
        {
            Character->CrosshairWidget->StopPlayFlipFlopButtonAnimation();
        }
        
        if(!bComingFromResponse)
        {
            CurrentNodeId = Dialogue[CurrentNodeId].Next_Node_ID;
        }
        bComingFromResponse = false;
        ShowObservation(Dialogue[CurrentNodeId].Text);
    }
}

void UObservableObjectComponent::ShowObservation(FString ObservationText)
{
    TempObsString = ObservationText;
    CurrentStringIndex = 0;
    bIsObsTyping = true;
    if (FmodAudioComponent)
    {
        FmodAudioComponent->Play();
    }

    DisplayResponseOptions();
}

void UObservableObjectComponent::StopSound() const
{
    if (FmodAudioComponent)
    {
        FmodAudioComponent->Stop();
    }
}

void UObservableObjectComponent::StopTyping() { bIsObsTyping = false; }

void UObservableObjectComponent::ResetObservation()
{
    bIsInteracting = false;
    bComingFromResponse = false;
    StopSound();
    StopTyping();
    CurrentStringIndex = 0;
}

// Called every frame
void UObservableObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bIsInteracting)
    {
        return;
    }

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

            if (CurrentStringIndex > Dialogue[CurrentNodeId].Text.Len())
            {
                bIsObsTyping = false;

                if (Dialogue[CurrentNodeId].Next_Node_ID != -1 && Dialogue[CurrentNodeId].Responses.Num() == 0)
                {
                    if (APristsWithGunsCharacter *Character =
                            Cast<APristsWithGunsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
                    {
                        Character->CrosshairWidget->StartPlayFlipFlopButtonAnimation();
                    }
                }

                CurrentStringIndex = 0;

                if (FmodAudioComponent)
                {
                    FmodAudioComponent->Stop();
                }
            }
            DelayAccumulator = 0;
        }
    }
}