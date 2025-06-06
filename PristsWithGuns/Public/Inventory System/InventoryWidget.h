// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UBaseItem;
class UCanvasPanel;
class UWrapBox;
class IItem;

/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

    virtual void NativeConstruct() override;
    virtual void OnWidgetRebuilt() override;

public:

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> CanvasPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWrapBox> WrapBox;

    void UpdateInventoryState(const TArray<TObjectPtr<UBaseItem>> &CurrentInventory) const;

};