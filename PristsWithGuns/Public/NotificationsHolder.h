// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "NotificationsHolder.generated.h"

class UNotification;
/********************************************************************
 *                      Notifications
 *         This widget is used by the notifications subsystem
 ********************************************************************/
UCLASS()
class PRISTSWITHGUNS_API UNotificationsHolder : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> NotificationVerticalBox;
    
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Observation")
    TSubclassOf<UUserWidget> NotificationWidgetClass;
    
    void PushInventoryAddNotification(FText ItemName);

    

};