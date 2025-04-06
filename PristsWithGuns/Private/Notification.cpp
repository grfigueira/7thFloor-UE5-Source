// Fill out your copyright notice in the Description page of Project Settings.


#include "Notification.h"

void UNotification::PlayInventoryAddRoutine(FText Item)
{
    UE_LOG(LogTemp, Display, TEXT("In PlayInventoryAddRoutine()"));
    if (!NotifText)
    {
        UE_LOG(LogTemp, Error, TEXT("NotifText is null in PlayInventoryAddRoutine"));
        return;
    }
    
    if (!Item.IsEmpty())
    {
        FString ItemString = Item.ToString();
        FString NotificationText = FString::Printf(TEXT("%s added to inventory!"), *ItemString);
        NotifText->SetText(FText::FromString(NotificationText));
    }
    else
    {
        // Fallback
        NotifText->SetText(FText::FromString(TEXT("Item added to inventory!")));
    }
    
    if (ItemNotificationRiseUp)
    {
        UE_LOG(LogTemp, Display, TEXT("Before PlayAnimation"));
        PlayAnimation(ItemNotificationRiseUp, 0, 1, EUMGSequencePlayMode::Forward, 1);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ItemNotificationRiseUp was null"));
    }
}