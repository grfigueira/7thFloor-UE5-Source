// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Templates/Function.h"
#include "CrosshairWidget.generated.h"

class UDialogOptionWidget;


/**
 *
 *   This is actually the main HUD widget.
 *   
 *   The name "CrosshairWidget" is incorrect and should be changed later.
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:

    
    
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* InteractionText;

    UPROPERTY(meta = (BindWidget))
    class UImage* NextObsPrompt;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* AnswerBox;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* GrowAnimation;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* PromptFlipFlop;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Observation")
    TSubclassOf<UDialogOptionWidget> ResponseWidgetClass;
    
    void AddDialogueOption(const FString& OptionText, TFunction<void()> OnClickedCallback);

    UFUNCTION(BlueprintCallable)
    void ClearDialogueOptions();
    
    
    UFUNCTION(BlueprintCallable)
    void SetInteractionText(const FText& NewText);
    

    UFUNCTION(BlueprintCallable)
    void PlayGrowAnimation(bool bReversed = false);

    UFUNCTION(BlueprintCallable)
    void StartPlayFlipFlopButtonAnimation();

    UFUNCTION(BlueprintCallable)
    void StopPlayFlipFlopButtonAnimation();
    
};
