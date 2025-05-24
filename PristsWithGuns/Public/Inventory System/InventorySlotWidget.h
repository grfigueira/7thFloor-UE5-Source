// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class AItemInspectionDisplay;
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

    UFUNCTION()
    virtual void NativeConstruct() override;

protected:
    
    UFUNCTION()
    void OnDoubleClick();
    
    UFUNCTION()
    void OnSlotButtonClicked();
    
public: 
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> CanvasPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> SlotButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> ItemImage;

    UPROPERTY(EditAnywhere, Category = "Item Slot")
    float DoubleClickTime = 2.0f;
    
    UFUNCTION()
    void SetItemInspectionData(const TSubclassOf<AItemInspectionDisplay>& _ItemInspectionClass, const FName& Name);

private:
    bool bWaitingForDoubleClick;
    FTimerHandle DoubleClickTimerHandle;
    
    UPROPERTY()
    TSubclassOf<AItemInspectionDisplay> ItemInspectionClass;

    UPROPERTY()
    FText ItemName;


};