// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory System/InventoryWidget.h"

#include "ItemInspectionDisplay.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Inventory System/InventorySlotWidget.h"
#include "Inventory System/BaseItem.h"
#include "Components/WrapBox.h"
#include "Inventory System/PlayerInventorySubsystem.h"


void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UInventoryWidget::OnWidgetRebuilt()
{
    Super::OnWidgetRebuilt();
    if (UPlayerInventorySubsystem* Inv = GetGameInstance()->GetSubsystem<UPlayerInventorySubsystem>())
    {
        UpdateInventoryState(Inv->GetHeldItems());
    }
    
}

// TODO I should change this to the old version 
void UInventoryWidget::UpdateInventoryState(const TArray<TObjectPtr<UBaseItem>> &CurrentInventory) const
{
    UE_LOG(LogTemp, Warning, TEXT("UpdateInventoryState called with %d inventory items"), CurrentInventory.Num());

    if (!ensure(WrapBox != nullptr))
    {
        UE_LOG(LogTemp, Error, TEXT("WrapBox is NULL! Cannot update inventory UI"));
        return;
    }

    TArray<UWidget *> SlotWidgets = WrapBox->GetAllChildren();
    UE_LOG(LogTemp, Warning, TEXT("Found %d slot widgets"), SlotWidgets.Num());

    for (int16 i = 0; i < SlotWidgets.Num(); ++i)
    {
        UE_LOG(LogTemp, Warning, TEXT("Processing slot %d"), i);

        UInventorySlotWidget *SlotWidget = Cast<UInventorySlotWidget>(SlotWidgets[i]);
        if (!SlotWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("Slot %d is NOT an InventorySlotWidget"), i);
            continue;
        }
        if(!ensure(SlotWidget->ItemImage))
        {
            continue;
        }

        // Assume empty by default
        float Opacity = 0.0f;
        FText ToolTip = FText::GetEmpty(); // item name
        UTexture2D *ItemTexture = nullptr;
        TSubclassOf<AItemInspectionDisplay> ItemDisplayClass = nullptr;

        if (i < CurrentInventory.Num() && CurrentInventory[i])
        {

            if (CurrentInventory[i]->SlotImage)
            {
                ItemTexture = CurrentInventory[i]->SlotImage;
                ToolTip = FText::FromName(CurrentInventory[i]->DisplayName);
                Opacity = 1.0f;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Item at slot %d has NULL image"), i);
            }

            if (CurrentInventory[i]->ItemDisplayClass)
            {
                ItemDisplayClass = CurrentInventory[i]->ItemDisplayClass;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Slot %d is out of inventory bounds or item is NULL"), i);
        }

        if (SlotWidget->ItemImage)
        {
            SlotWidget->ItemImage->SetRenderOpacity(Opacity);

            if (ItemTexture)
            {
                SlotWidget->ItemImage->SetBrushFromTexture(ItemTexture);
            }

            if (ItemDisplayClass)
            {
                SlotWidget->SetItemInspectionData(ItemDisplayClass, CurrentInventory[i]->DisplayName);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Bad!! ItemDisplayClass was NULL in UpdateInventoryState"));
            }
        }

        if (SlotWidget->SlotButton && !ToolTip.IsEmpty())
        {
            SlotWidget->SlotButton->SetToolTipText(ToolTip);
        }
        else if (!SlotWidget->SlotButton && !ToolTip.IsEmpty())
        {
            UE_LOG(LogTemp, Error, TEXT("SlotButton is NULL in slot %d"), i);
        }
    }
}