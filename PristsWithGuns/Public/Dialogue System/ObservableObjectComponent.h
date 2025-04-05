// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogOptionWidget.h"
#include "Interfaces/IInteractable.h"
#include "DialogueParser.h"
#include "Components/ActorComponent.h"
#include "ObservableObjectComponent.generated.h"

class UDialogueCallbackHandlerSubsystem;
class UCrosshairWidget;
class UFMODAudioComponent;
class UFmodEvent;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PRISTSWITHGUNS_API UObservableObjectComponent : public UActorComponent, public IInteractable
{
    GENERATED_BODY()

public:
    UObservableObjectComponent();

    UPROPERTY(EditAnywhere, Category = "Dialogue",
        meta = (Tooltip = "The text that shows up in the HUD when looking at an interactable"))
    FString HoverPopupText;

    UPROPERTY(EditAnywhere, Category="Dialogue", meta=(Tooltip="The file should be located in Content/Data/"))
    FString JsonFileName;

    // True if the player pressed 'e' and the first observations is being displayed
    bool bIsInteracting;

protected:
    TMap<int32, FDialogueNode> Dialogue;

    UPROPERTY(EditAnywhere, Category="Observation")
    float Speed = 70;

    UPROPERTY(EditAnywhere, Category="Observation")
    UFmodEvent *SpeechSoundEvent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound")
    UFMODAudioComponent *FmodAudioComponent; // typing sound

public:
    UFUNCTION()
    void NextObservation();

    UFUNCTION()
    void ShowObservation(FString ObservationText);

    UFUNCTION()
    void StopSound() const;

    UFUNCTION()
    void StopTyping();

    // Use this function when the player is no longer interested in "observing" the object
    // this will reset most parameters to initial default, stop typing sound, and all that stuff.
    UFUNCTION()
    void ResetObservation();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;
    virtual void BeginPlay() override;

    virtual void Interact(class ACharacter *Character) override;
    virtual FText GetInteractionText() const override;
    void DisplayResponseOptions();
    void DisplayNextResponse();

private:
    TObjectPtr<UDialogueCallbackHandlerSubsystem> CallbackHandlerSubsystem; 
    UCrosshairWidget *CrosshairWidget;
    int32 CurrentNodeId;
    bool bIsObsTyping;
    float DelayAccumulator;
    int32 CurrentStringIndex;
    FString TempObsString;
    bool bComingFromResponse;

private:
    int32 CurrentResponseIndex;
    FTimerHandle UnusedHandle;

};