// Fill out your copyright notice in the Description page of Project Settings.


#include "NotificationsHolder.h"

#include "FMODBlueprintStatics.h"
#include "Notification.h"
#include "Components/VerticalBoxSlot.h"

void UNotificationsHolder::PushInventoryAddNotification(FText ItemName)
{
    UNotification* NotificationWidget = CreateWidget<UNotification>(GetWorld(), NotificationWidgetClass);
    
    if (UVerticalBoxSlot* NewSlot = NotificationVerticalBox->AddChildToVerticalBox(NotificationWidget))
    {
        NewSlot->SetHorizontalAlignment(HAlign_Center);
        NewSlot->SetVerticalAlignment(VAlign_Center);
        NewSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
        NewSlot->SetPadding(FMargin(0, 0, 0, 0));
        
        NotificationWidget->PlayInventoryAddRoutine(ItemName);
        if(NotificationSound)
        {
            UFMODBlueprintStatics::PlayEvent2D(GetWorld(), NotificationSound, true);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Couldn't find NotificationSound"));
        }
        
    }
}