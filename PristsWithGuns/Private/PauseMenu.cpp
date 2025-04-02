// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"

#include "Components/Button.h"
#include "PristsWithGuns/PristsWithGunsCharacter.h"

void UPauseMenu::NativeConstruct()
{
    Super::NativeConstruct();
    if(ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::OnResumeButtonClicked);
    }
}

void UPauseMenu::OnResumeButtonClicked()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            // Get the pawn and cast it
            if (APristsWithGunsCharacter* ActualCharacter = Cast<APristsWithGunsCharacter>(PlayerController->GetPawn()))
            {
                ActualCharacter->UnpauseGame();
            }
        }
}

void UPauseMenu::FadeIn()
{
    AddToViewport();
    PlayAnimation(PauseMenuFade, 0, 1, EUMGSequencePlayMode::Forward, 1);
}

void UPauseMenu::FadeOut()
{
    AnimationFinishedEvent.BindDynamic(this, &UPauseMenu::OnFadeOutFinished);

    BindToAnimationFinished(PauseMenuFade, AnimationFinishedEvent);
    PlayAnimation(PauseMenuFade, 0, 1, EUMGSequencePlayMode::Reverse, 1);
}

void UPauseMenu::OnFadeOutFinished()
{
    RemoveFromParent();
    UnbindFromAnimationFinished(PauseMenuFade, AnimationFinishedEvent);
}
