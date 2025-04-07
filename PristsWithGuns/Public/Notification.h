// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Notification.generated.h"

/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UNotification : public UUserWidget
{
    GENERATED_BODY()

protected:
    
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* ItemNotificationRiseUp;
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* NotifText;

public:
    UFUNCTION()
    void PlayInventoryAddRoutine(FText Item);
    
    UFUNCTION()
    void OnAnimationEnd();

};