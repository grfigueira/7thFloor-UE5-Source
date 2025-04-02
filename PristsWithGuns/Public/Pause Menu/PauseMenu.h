// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UCanvasPanel;
class UButton;
/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UPauseMenu : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    UFUNCTION()
    void OnResumeButtonClicked();

    UPROPERTY()
    FWidgetAnimationDynamicEvent AnimationFinishedEvent;
    
public:
    UFUNCTION()
    void FadeIn();

    UFUNCTION()
    void FadeOut();

    UFUNCTION()
    void OnFadeOutFinished();
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> CanvasPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ResumeButton;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    TObjectPtr<UWidgetAnimation> PauseMenuFade;

};