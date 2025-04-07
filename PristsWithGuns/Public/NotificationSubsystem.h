// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NotificationSubsystem.generated.h"

class UFmodEvent;
class UNotificationsHolder;

/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UNotificationSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    TObjectPtr<UNotificationsHolder> NotificationsHolder;

public:
    void Initialize(FSubsystemCollectionBase &Collection) override;
    virtual void Deinitialize() override;

    void RegisterNotificationWidget(TObjectPtr<UNotificationsHolder> NotifHolderWidget);
    
    UFUNCTION(BlueprintCallable)
    void PushInventoryAddNotification(FText ItemName);

};