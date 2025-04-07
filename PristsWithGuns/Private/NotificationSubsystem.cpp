// Fill out your copyright notice in the Description page of Project Settings.


#include "NotificationSubsystem.h"

#include "NotificationsHolder.h"

void UNotificationSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
    
}

void UNotificationSubsystem::Deinitialize()
{
    Super::Deinitialize();
}
     
void UNotificationSubsystem::RegisterNotificationWidget(TObjectPtr<UNotificationsHolder> NotifHolderWidget)
{
    NotificationsHolder = NotifHolderWidget;
    if(NotificationsHolder)
    {
        NotificationsHolder->AddToViewport();
    }
}

void UNotificationSubsystem::PushInventoryAddNotification(FText ItemName)
{
    if(NotificationsHolder)
    {
        NotificationsHolder->PushInventoryAddNotification(ItemName);

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Tried pushing a notification but the NotificationsHolder Widget is null!"));
    }
}
