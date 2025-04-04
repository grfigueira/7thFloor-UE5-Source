// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory System/InventoryWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Inventory System/InventorySlotWidget.h"
#include "Inventory System/BaseItem.h"
#include "Components/WrapBox.h"

/*
void UInventoryWidget::UpdateInventoryState(const TArray<TObjectPtr<UBaseItem>> &CurrentInventory) const
{
    if(!WrapBox)
    {
        TArray<UWidget*> SlotWidgets = WrapBox->GetAllChildren();

        for (int16 i = 0; i < SlotWidgets.Num(); ++i)
        {
            if(UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(SlotWidgets[i]))
            {
                if(CurrentInventory[i] && CurrentInventory[i]->SlotImage)
                {
                    SlotWidget->ItemImage->SetBrushFromTexture(CurrentInventory[i]->SlotImage);
                    SlotWidget->ItemImage->SetRenderOpacity(1.0f);
                }
                else
                {
                    SlotWidget->ItemImage->SetRenderOpacity(0.0f);
                }
                
            }
        }
    }
}
*/

void UInventoryWidget::UpdateInventoryState(const TArray<TObjectPtr<UBaseItem>>& CurrentInventory) const
{
    UE_LOG(LogTemp, Warning, TEXT("UpdateInventoryState called with %d inventory items"), CurrentInventory.Num());
    
    if(WrapBox)
    {
        UE_LOG(LogTemp, Warning, TEXT("WrapBox is valid"));
        TArray<UWidget*> SlotWidgets = WrapBox->GetAllChildren();
        UE_LOG(LogTemp, Warning, TEXT("Found %d slot widgets"), SlotWidgets.Num());

        for (int16 i = 0; i < SlotWidgets.Num(); ++i)
        {
            UE_LOG(LogTemp, Warning, TEXT("Processing slot %d"), i);
            if(UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(SlotWidgets[i]))
            {
                UE_LOG(LogTemp, Warning, TEXT("Slot %d is an InventorySlotWidget"), i);
                
                if(i < CurrentInventory.Num())
                {
                    if(CurrentInventory[i])
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Item at slot %d exists"), i);
                        if(CurrentInventory[i]->SlotImage)
                        {
                            UE_LOG(LogTemp, Warning, TEXT("Item at slot %d has valid image"), i);
                            SlotWidget->ItemImage->SetBrushFromTexture(CurrentInventory[i]->SlotImage);
                            SlotWidget->SlotButton->SetToolTipText(FText::FromName(CurrentInventory[i]->DisplayName));
                            SlotWidget->ItemImage->SetRenderOpacity(1.0f);
                        }
                        else
                        {
                            UE_LOG(LogTemp, Warning, TEXT("Item at slot %d has NULL image"), i);
                            SlotWidget->ItemImage->SetRenderOpacity(0.0f);
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Item at slot %d is NULL"), i);
                        SlotWidget->ItemImage->SetRenderOpacity(0.0f);
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Slot %d is out of inventory bounds"), i);
                    SlotWidget->ItemImage->SetRenderOpacity(0.0f);
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Slot %d is NOT an InventorySlotWidget"), i);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("WrapBox is NULL! Cannot update inventory UI"));
    }
}