// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UCanvasPanel;
class UButton;
class UImage;
/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UInventorySlotWidget : public UUserWidget
{
    GENERATED_BODY()

public: 
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> CanvasPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> SlotButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> ItemImage;


};