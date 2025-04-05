// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairWidget.h"

#include "Dialogue System/DialogOptionWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"


void UCrosshairWidget::AddDialogueOption(const FString &OptionText, TFunction<void()> OnClickedCallback)
{
    if(!ResponseWidgetClass)
    {
        return;
    }
    UDialogOptionWidget* ResponseWidget = CreateWidget<UDialogOptionWidget>(GetWorld(), ResponseWidgetClass);
    
    ResponseWidget->OptionText->SetText(FText::FromString(OptionText));
    ResponseWidget->ClickedCallback = OnClickedCallback;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, ResponseWidget->OptionText->GetText().ToString());
    
    if (UVerticalBoxSlot* NewSlot = AnswerBox->AddChildToVerticalBox(ResponseWidget))
    {
        NewSlot->SetHorizontalAlignment(HAlign_Center);
        NewSlot->SetVerticalAlignment(VAlign_Center);
        NewSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
        NewSlot->SetPadding(FMargin(0, 0, 0, 10));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Couldn't add ResponseWidget to AnswerBox!"));
    }
}

void UCrosshairWidget::ClearDialogueOptions()
{
    AnswerBox->ClearChildren();
}

void UCrosshairWidget::SetInteractionText(const FText &NewText)
{
    if(InteractionText)
    {
        InteractionText->SetText(NewText);
    }
}

void UCrosshairWidget::PlayGrowAnimation(const bool bReversed)
{
    if (GrowAnimation)
    {
        if(!bReversed)
        {
            PlayAnimationForward(GrowAnimation, 1);
        }
        else
        {
            PlayAnimationReverse(GrowAnimation, 1);
        }
    }
}

void UCrosshairWidget::StartPlayFlipFlopButtonAnimation()
{
    if(PromptFlipFlop)
    {
        PlayAnimation(PromptFlipFlop, 0, 0, EUMGSequencePlayMode::Forward, 1);
    }
}

void UCrosshairWidget::StopPlayFlipFlopButtonAnimation()
{
    if(PromptFlipFlop)
    {
        if(NextObsPrompt)
        {
            NextObsPrompt->SetColorAndOpacity(FLinearColor(1, 1, 1, 0));
        }
        StopAnimation(PromptFlipFlop);
    }
}